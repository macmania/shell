/**
* starter file to test signal handling mechanism to be put in place inside Shell.c 
* This file is meant to comprise only in 
   - handling signal mechanism 
   - addition of processes in JobControl 
   - deletion of process in JobControl when the user sends a SIGINT signal to the 
   	 process

   This starter file lack the ability to interact with the user
*/


#include "/Users/jojofabe/Workspace/c/shell/JobControl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

/*Signal Handlers to be implemented**/
void signalHandler(int, siginfo_t*, void*);
//void signalHandlerList(int, siginfo_t*, void*);


/*global variables**/
pid_t child, parent, status;


int main(void){
	//temporary commands to be used
	char* args_ls[3]; 	args_ls[0]="ls"; 		args_ls[1]="-l"; 	   args_ls[2]=NULL;
	char* args_cd[2]; 	args_cd[0]="cd"; 		args_ls[1]=NULL;
	char* args_man[2]; 	args_man[0]="man"; 		args_man[1]=NULL;
	char* args_echo[3]; args_echo[0]="echo";	args_echo[1]="$PATH";  args_echo[2]=NULL;
	//To-do char* args_nano[3]; args_nano[0]="nano"; 	args_nano[1]="HiHi";   args_nano[2]=NULL;

	//initialize the signal handlers here
	struct sigaction sa; //examine data race conditions if there are any

	sa.sa_sigaction = signalHandler;
	sigemptyset(&sa.sa_mask); //helps block signals
	sa.sa_flags = SA_RESTART; 

	sigaction(SIGCHLD, &sa, NULL); //a child process being done
  	sigaction(SIGCONT, &sa, NULL); //a process that is sent to be continued
  	sigaction(SIGTTIN, &sa, NULL); //terminal access signal
  	sigaction(SIGTTOU, &sa, NULL);
  	sigaction(SIGINT, &sa, NULL);
  	sigaction(SIGSTOP, &sa, NULL);

	while(1){


	}

	exit(0);
}



