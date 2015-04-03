//Small project as a shell
/***
  Part 1: build a small scale shell that handles all of the commands just like a
  unix like shell (be able to handle fork and parent stuff)
  Part 2: handle multi-threading
**/
#include <stdio.h>
#include <stdlib.h>
#include "shell.h" //to-do need to put this in a directory
#include <unistd.h>//#include "parser.h"
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

int main (int argc, char** argv) {
  printPrompt();
  //add a list that adds commands to the list
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
        childPid = fork();
        if(childPid == 0) {
          execCmd(cmd); //calls execvp
        } else {
          if(isBgJob(cmd)) {
            //record in a list of background jobs
          } else {
            waitPid(childPid);
          }
        }
    }
  }
}


void waitPid(int pid) {

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
  
  else {
    printErrMsg(LESS_NUM_ARGS, command); 
  }


 
}



void execCmd(struct parseInfo* cmd) {

}

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