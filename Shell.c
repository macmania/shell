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

//Global variables that keep track of parent group id and child id
//pid_t parentId, childID, status; 
	/**potential race condition here, might need to look this further**/
            //data-race condition

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
  sa.sa_sigaction = signalHandler;
  sigemptyset(&sa.sa_mask);  /* Block other terminal-generated signals while handler runs. */
  sa.sa_flags = SA_RESTART; /* restart function if interrupted by handler */

  //this deals with signals associated with 
  sigaction(SIGCHLD, &sa, NULL); //a child process being done
  sigaction(SIGCONT, &sa, NULL); //a process that is sent to be continued
  sigaction(SIGTTIN, &sa, NULL); //terminal access signal
  sigaction(SIGTTOU, &sa, NULL); //terminal access signal

  //The list needs to be added and removed, make sure no signals
  //are blocked
  saList.sa_sigaction = &signalHandlerList;
  sigemptyset(&sa.sa_mask);

  /*Block all other terminal-generated signals**/
  sigaddset(&sa.sa_mask, SIGCHLD); //child has exited or terminated
  sigaddset(&sa.sa_mask, SIGCONT); //tells the process to continue processing
  sigaddset(&sa.sa_mask, SIGTTIN); //bg process attempts to read from terminal, this is sent to that process
  sigaddset(&sa.sa_mask, SIGTTOU); /**bg process attempts to write from terminal or set its terminal modes
  	  	  	  	  	  	  	  	    , this is sent to that process **/

  saList.sa_flags = SA_RESTART;
  sigaction(SIGINT, &saList, NULL);
  sigaction(SIGSTOP, &saList, NULL);


  int isShellInteractive = 1;

  //Temporary testing the shell.
  if(isShellInteractive){
	  /*this is testing purposes solely to test the validity of
	   * signal handler for SIGCHLD, SIGCONT, SIGTTIN, SIGTTOU,
	   * SIGINT, SIGSTOP
	  */

	  /** Wait, for-loop*/
	  while(1);


	  /*Make 5 jobs
	   * - write out the process ID to the terminal for each of the jobs
	   * - Ctrl-Z the first job (this job is now in the foreground and should be saved in the list)
	   * - continue with the next job based on the array
	   * - when the job terminates by (wait) and/or the job is sent Ctrl-C (for loop)
	   *   this job should be removed from the job_control list and printed out in the terminal
	   *	 - when the user types the PID of the stopped job, the OS sends a sigcont signal,
	   *	   thus, this process needs to be removed from the job_control list
	   *
	   * To-do: need to investigate how to test out SIGTTOU, SIGTTIN.
	   *
	   **/
  }

  else{
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
				  waitpid((pid_t)(-1), 0, WNOHANG);
			  }
			}
		}
	  }
  }
}

//provides more context to the group id and such
/**
 * Needs more work
 * */
void signalHandler(int s, siginfo_t *si, void *context) {
  switch(s){ //handles all of the signals
    case SIGCHLD: //Running -> Terminated (history)
    		while (waitpid((pid_t)(-1), 0, WNOHANG) > 0); //reaps the child process
    		//wait until the child process is done and delete it from the list
    		/*Temporary: finds the pid of this job and removes the job from the list
    		 * and/or change the field*/
      break;
    case SIGCONT:  //Ready -> Running
    	   //finds the process and send some sort of signal
    		/*To-do send the signal to the job and add it to the list */
      break; 
    case SIGTTIN: 
    		/*To-do, process attempts to read from the terminal, the default action is
    		 * to terminate the process**/
      break; 
    case SIGTTOU:
      /* To-do, process attempts to write to the terminal or change the termios, default action
       * is to terminate the process
       */
      break;
  }
}

/*
 * Still need to further expand this part
 * **/
void signalHandlerList(int s, siginfo_t *si, void *context) {
  switch(s){
    case SIGINT: //Running -> Terminate
    		//kill this job and remove from the list
    		/* To-do, removes the job from the list in job_control
    		 * Temporary: add the job in an array of jobs or so that changes
    		 * dynamically, this signals whether */
    	printf("Hi");
    	break;
    case SIGSTOP: //Running -> Background/Foreground
    		//put in foreground
    		/* To-do, find the job from the list in job_control **/
    	  printf("Hello");
      break;
  }
}

void printPrompt(){
  printf("Welcome to Jojo's small scale shell\n");
}

/* Reads line in command line */
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

//Still need to test this and figure out how to add this in the job
//pipeline
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
          break;
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
//(Running)
void launchProcess(struct parseInfo* cmd) {
  
}

/*** Need to change this portion here, argument is not consistent**/
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
