//Small project as a shell
/***
  Part 1: build a small scale shell that handles all of the commands just like a
  unix like shell (be able to handle fork and parent stuff)
  Part 2: handle multi-threading
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "JobControl.h"
#include "Shell.h" //to-do need to put this in a directory
#include "Parser.h"

//Global variables that keep track of parent group id and child id
//pid_t parentId, childID, status; 
//potential race condition here, might need to look this further
            //data-race condition??
void signalHandler(int, siginfo_t*, void*);
void signalHandlerList(int, siginfo_t*, void*);


int main (int argc, char** argv) {
  struct sigaction sa; 
  struct sigaction saList; //this sigaction deals with sigint and sigstp
                        //ensures that data is not touched
  printPrompt();
  //add a list that adds commands to the list

  //These are the signals that need to be handled appropriately
  //by the job scheduler
  sa.sa_handler = signalHandler;
  sigemptyset(&sa.sa_mask);  /* Block other terminal-generated signals while handler runs. */
  sa.sa_flags = SA_RESTART; /* restart function if interrupted by handler */

  //this deals with signals associated with 
  sigaction(SIGCHLD, sa, NULL); //a child process being done
  sigaction(SIGCONT, sa, NULL); //a process that is sent to be continued
  sigaction(SIGTTIN, sa, NULL); //terminal access signal
  sigaction(SIGTTOU, sa, NULL); //terminal access signal

  //The list needs to be added and removed, make sure no signals
  //are blocked
  saList.sa_handler = signalHandlerList;
  sigemptyset(&sa.sa_mask);

  /*Block all other terminal-generated signals**/
  sigaddset(&block_mask, SIGCHLD);
  sigaddset(&block_mask, SIGCONT);
  sigaddset(&block_mask, SIGTTIN);
  sigaddset(&block_mask, SIGTTOUT);

  saList.sa_flags = SA_RESTART;
  sigaction(SIGINT, signalHandlerList, NULL);
  sigaction(SIGSTP, signalHandlerList, NULL);

  while(1) {
    int childPid; 
    char* cmdLine;
    struct parseInfo* cmd; 
    commandType* cmdType; 

    cmdLine = readCmdLine(); //tokenizes the commands
    //perhaps record the commands typed in a document
    if(isCmdEmpty(cmdLine)){
      continue; 
    }
        
    cmd = parse(cmdLine); 
    parse_command(cmdLine, cmdType); //takes the command and saves them to cmdType

    if (isBltInCmd(cmd)){
      execBltInCmd(cmd); //stop, etc.
    } 
    else {
        //need to put this somewhere else
        childPid = fork();
        if(childPid == 0) {
          launchProcess(cmd); //calls execvp
        } else {
          if(isBgJob(cmd)) {
            //record in a list of background jobs
          } else {
            waitpid(childPid);
          }
        }
    }
  }
}

//provides more context to the group id and such
void signalHandler(int s, siginfo_t *si, void *context) {
  switch(s){ //handles all of the signals
    case SIGCHLD:
      //wait until the child process is done and delete it from the list
      break;
    case SIGCONT: 
      //send the signal to the job and add it to the list
      break; 
    case SIGTTIN: 
      //determine the termios
      break; 
    case SIGTTOU:
      //determine the termios
      break;
  }
}


void signalHandlerList(int s, siginfo_t *si, void *context) {
  switch(s){
    case SIGINT: //kill this job and remove from the list
      break;
    case SIGSTP: //put in foreground
      break;
  }
}



void printPrompt(){
  printf("Welcome to Jojo's small scale shell\n");
}

char* readCmdLine(void){
  char* line = malloc(100), *linep = line;
  size_t lenMax = 100, len = lenMax;
  int c;

  if(line == NULL){
    return NULL;
  }
  else {
    while((c = fgetc(stdin)) != EOF){

      if(--len == 0){ //resize the size
        len = lenMax;
        lenMax = lenMax * 2;
        char* lineNew = realloc(linep, lenMax);
        if(!lineNew){
          free(linep);
          return NULL; //no more memory available
        }

        line = lineNew + (line - linep);
        linep = lineNew;
      }

      if((*line++ = c) == '\n'){
        break;
      }
    }

    //*line = '\0';
    return linep;
  }
}

//Still need to test this
void execBltInCmd(struct parseInfo* cmd) {
  char* command = cmd->command;
  char** arg = cmd->ArgVarList;
  int argNum = cmd->argVarNum; 


  if(strcmp(command, "kill") == 0){
    if(argNum == 0) {
      printErrMsg(MORE_NUM_ARGS, command);
      return;
    }
    else if (argNum > 1){
      printErrMsg(LESS_NUM_ARGS, command);
    }
      
    char* strPtrEnd;
    //kills the given job_pid
      int jobID = strtol(arg[0], &strPtrEnd, 10); 
      //this part is shaky, need to be more rigorous
      if(strPtrEnd != NULL){
        printErrMsg(NEED_NUMERIC_ARG, command);
        return;
      }

      int errNum = kill(jobID, SIGINT); 
      switch (errNum){
        case EINVAL: 
          printf("Signal number not supported\n");
          break; 
        case EPERM: 
          printf("Process does not have permission to send signal to any receiving process\n"); 
          break; 
        case ESRCH: 
          printf("No process or process group can be found corresponding to pid: %d\n", jobID); 
          break; 
        default: 
          printf("Error %d due to kill command\n", errNum);
      }
  }

  if(strcmp(command, "cd") == 0){
    if(argNum > 2){
      printErrMsg(LESS_NUM_ARGS, command);
      return; 
    }

    struct stat buffer;
    int status;
    char path[strlen(getenv("HOME"))+10];

    if(argNum != 0){
      status = stat(arg[0], &buffer);
      if(!S_ISDIR(buffer.st_mode) ){
        printf("The argument: %s passed is not a valid directory\n", arg[0]);
        return;
      }
      strcpy(path, arg[0]);
      //change the directory 
    }
    else {
      strcpy(path, getenv("HOME"));
      strcat(path, "/Workspace");
    }

    if(chdir(path) == -1){
      printf("Command cd did not work\n");
    }
    
    return;
  }

  if(argNum == 0){

    if(strcmp(command, "exit") == 0){
      exit(EXIT_SUCCESS); //temporary :) 
      //kills of the jobs in the list
    }
    else if(strcmp(command, "jobs") == 0){
      //need to have a running list that has the list of jobs that are running on the background, etc. 
      //prints all of the current and background jobs
    }

    else if(strcmp(command, "history") == 0){
      //need to have a queue of the number of jobs that have been processed successfully
      //prints all of the jobs that are valid
    }

  }
  
  else 
    printErrMsg(LESS_NUM_ARGS, command); 
}


//call execvp here 
void launchProcess(struct parseInfo* cmd) {
  
}

/*** Need to change this portion here **/
bool isBgJob(struct parseInfo* cmd) {
  return false; 
}


//temporary, only built in command is exit for now
bool isBltInCmd(struct parseInfo* cmd){
  char* command = cmd->command;
  if(strcmp(command, "exit") == 0 || strcmp(command, "jobs") == 0 ||
       strcmp(command, "cd") == 0 || strcmp(command, "kill") == 0 || 
          strcmp(command, "history") == 0){
    return true;
  }
  return false; 
}

bool isCmdEmpty(char* cmd){
  if(cmd == NULL || cmd[0] == '\0' || cmd[0] == '\n'){
    return true; 
  }
  return false; 
}

//prints error message based on the error message built in 
void printErrMsg(enum Error_Messages msg, char* cmd) {
  printf("%s: ", cmd);

  if(msg == LESS_NUM_ARGS) {
    printf("incorrect number of arguments passed"); 
  }
  else if(msg == MORE_NUM_ARGS) {
    printf("need more information to run command"); 
  }
  else if(msg == NEED_NUMERIC_ARG) {
    printf("numergic argument required"); 
  }
  else if(msg == FILE_NAME_INCOR){
    printf("No such file or directory"); 
  }
  printf("\n");
}