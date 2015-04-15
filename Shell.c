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
static pid_t shell_pgid;
static int shell_fd, shell_interactive;
static int sizeStoppedJobs;
struct termios shell_tmodes;

int main (int argc, char** argv) {

  init();
  printPrompt();
  int childPid, pPid;
  while(TRUE) {

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
//set up the shell as its own process group
void init(void){
	pid_t pid;

	shell_fd = STDIN_FILENO; //file descriptor for terminal
	shell_interactive = isatty(shell_fd); //tests whether it's a fd to terminal

	if(shell_interactive){
	    while (tcgetpgrp (shell_fd) != (shell_pgid = getpgrp ()))
	        kill (pid, SIGTTIN);

	    //ignore these signals
	    signal(SIGTSTP, SIG_IGN);
	    signal(SIGTTIN, SIG_IGN);
	    signal(SIGTTOU, SIG_IGN);

		struct sigaction sa;
		struct sigaction saChld;

		sa.sa_sigaction = sigHandlers;
		sigemptyset(&sa.sa_mask);  /* Block other terminal-generated signals while handler runs. */
		sa.sa_flags = SA_RESTART; /* restart function if interrupted by handler */

		saChld.sa_sigaction = &saChld;
		sigemptyset(&saChld.sa_mask);
		saChld = SA_RESTART;

		//this deals with signals associated with
		sigaction(SIGSTOP, &sa, NULL);
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGCONT, &sa, NULL);
		sigaction(SIGCHLD, &saChld, NULL);

		shell_pgid = getpid();
		if(setpgid(shell_pgid, shell_pgid) < 0){
			perror("Couldn't put shell in its own group");
			exit(EXIT_FAILURE);
		}

		tcsetgrp(shell_fd, shell_pgid);
		tcgetattr(shell_fd, &shell_tmodes);
	}
	else{
		printf("Error in initializing shell. Exiting...\n");
		exit(EXIT_FAILURE);
	}
}


/*Signal Handlers**/
void sigChldHandler(int sig, siginfo_t *si, void *context){
	pid_t pid;
	int status;

	pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);

	if(pid > 0){ //handle the
		job *j = getJob(pid, BY_PROCESS_ID);
		if(j == NULL){
			printf("Job is null inside sig child handler"); //temporary debug print info
			return;
		}
		else if(WIFEXITED(status)){ //
			set_job_completed(j);
			printf("Job done");
			print_info(j->command);
			printf("\n");
			delete_job(j); //To-do change delete_job method in JobControl.c
		}
		else if(WIFSIGNALED(status)){ //delete job
			printf("The job below was not handled properly\n");
			print_info(j->command);
			delete_job(j); //temporary fix
		}
		else if(WIFSTOPPED(status)){ //child process has been sent stopped signal
			set_job_background(j);
			sizeStoppedJobs++;
		}
		else if(WIFCONTINUED(status)){
			set_job_continued(j);
			kill(j->pgid, SIGCONT); //iffy
		}
	}
}


void sigHandlers(int sig, siginfo_t *si, void *context) {
	int pid = getpid(); //the pid of the child
	job* j = getJob(pid, BY_PROCESS_ID);

	switch(sig){
		case SIGCONT:  //Ready -> Running
		  set_job_continued(j);

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
		case SIGINT:

		 break;
  }
}


void printPrompt(){
  printf("*** Welcome to Jojo's small scale shell\n\n ****");
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

int isCmdEmpty(char* cmd){
  if(cmd == NULL || cmd[0] == '\0' || cmd[0] == '\n'){
    return TRUE;
  }
  return FALSE;
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
