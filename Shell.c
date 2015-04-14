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
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "Shell.h"

static pid_t shell_pid;
static pid_t shell_gpid;
static int shell_fd, shell_interactive;
static int sizeStoppedJobs;

int main (int argc, char** argv) {

  init();
  printPrompt();
  int childPid, pPid;
  while(true) {

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
  		  //To-do
  		  launchProcess(cmd); //calls execvp
  		} else {
  		  pPid = waitpid(WAIT_ANY, 0, WNOHANG);

  		  if(EINTR == pPid)
  			  printf("signal interrupt delivered to calling process");
  		}
  	}
	  
  }
}

//initialize signal handlers
void init(void){
	pid_t pid;

	pid = getpid();
	/*Set termios and such, just set setgpid**/

	struct sigaction sa;
	struct sigaction saChld;

	sa.sa_sigaction = sigHandlers;
	sigemptyset(&sa.sa_mask);  /* Block other terminal-generated signals while handler runs. */
	sa.sa_flags = SA_RESTART; /* restart function if interrupted by handler */

	//this deals with signals associated with
	sigaction(SIGCHLD, &sa, NULL); //a child process being done
	sigaction(SIGCONT, &sa, NULL); //a process that is sent to be continued
	//sigaction(SIGTTIN, &sa, NULL); //terminal access signal
	//sigaction(SIGTTOU, &sa, NULL); //terminal access signal

	//The list needs to be added and removed, make sure no signals
	//are blocked
	saList.sa_sigaction = &signalHandlerList;
	sigemptyset(&sa.sa_mask);

	/*Block all other terminal-generated signals**/
	sigaddset(&sa.sa_mask, ); //child has exited or terminated
	sigaddset(&sa.sa_mask, SIGCONT); //tells the process to continue processing
	sigaddset(&sa.sa_mask, SIGTTIN); //bg process attempts to read from terminal, this is sent to that process
	sigaddset(&sa.sa_mask, SIGTTOU); /**bg process attempts to write from terminal or set its terminal modes
									, this is sent to that process **/
	sigaction(SIGSTOP, &saChld, NULL);
	saChld.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &saChld, NULL);



}


/*Signal Handlers**/
void sigChldHandler(int sig, siginfo_t *si, void *context){
	pid_t pid;
	int status;

	pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);

	if(pid > 0){ //handle the
		job *j = getJob(pid, BY_PROCESS_PID);
		if(j == NULL){
			printf("Job is null inside sig child handler"); //temporary debug print info
			return;
		}
		else if(WIFEXITED(status)){ //

			set_job_completed(j);
			print_info(j->command)
		}
		else if(WIFSIGNALED(status)){ //

		}
		else if(WIFSTOPPED(status)){ //child process has been sent stopped signal

		}
		else if(WIFCONTINUED(status)){

		}
	}
}


void sigHandlers(int sig, siginfo_t *si, void *context) {
  switch(sig){ //handles all of the signals
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
    case SIGINT: //Running -> Terminate
       		//kill this job and remove from the list
       		/* To-do, removes the job from the list in job_control
       		 * Temporary: add the job in an array of jobs or so that changes **/
     break;

  }
}

/*
 * Still need to further expand this part
 * **/
void signalHandlerList(int s, siginfo_t *si, void *context) {
  switch(s){

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

//temporary, only built in command is exit for now
int isBltInCmd(struct parseInfo* cmd){
  char* command = cmd->command;
  if(strcmp(command, "exit") == 0 || strcmp(command, "jobs") == 0 ||
       strcmp(command, "cd") == 0 || strcmp(command, "kill") == 0 || 
          strcmp(command, "history") == 0){
    return TRUE;
  }
  return FALSE;
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
