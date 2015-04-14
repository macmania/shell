/**
* starter file to test signal handling mechanism to be put in place inside Shell.c 
* This file is meant to comprise only in 
   - handling signal mechanism 
   - addition of processes in JobControl 
   - deletion of process in JobControl when the user sends a SIGINT signal to the 
   	 process

   This starter file lack the ability to interact with the user
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define MAX_NUM 10


typedef struct commands { 
	char* args[MAX_NUM];
	int pid; 
	int bg; 
	int complete; 
}commands;


/*Signal Handlers to be implemented**/
void signalHandler(int, siginfo_t*, void*);
//void signalHandlerList(int, siginfo_t*, void*);
void init_commands(void);
void launch_job(void);
void findJob(void);
void printCommand(void);
/*global variables**/
pid_t child, parent, status;
struct commands* cmd[MAX_NUM]; 
int pointer_cmd, jobs_completed; 

int main(void){

	//Signal handling
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
  	
  	init_commands(); 

	while(1){
		child = fork(); 
		if(child == 0){
			cmd[pointer_cmd]->pid = child;
			launch_job();
		}

		if(jobs_completed == 4) //all jobs were completed
			exit(EXIT_SUCCESS);
	}

	int i;
	for(i = 0; i < 4; i++)
		free(cmd[i]);

	exit(0);
}

void init_commands(void){
	cmd[0] = malloc(sizeof(struct commands));
	cmd[1] = malloc(sizeof(struct commands));
	cmd[2] = malloc(sizeof(struct commands));
	cmd[3] = malloc(sizeof(struct commands));

	cmd[0]->args[0] = "ls"; cmd[0]->args[1] = NULL; 
	cmd[1]->args[0] = "whois"; cmd[1]->args[1] = "jojofabe"; cmd[1]->args[2] = NULL; 
	cmd[2]->args[0] = "man"; cmd[2]->args[1] = NULL; 
	cmd[3]->args[0] = "echo"; cmd[3]->args[1] = "$PATH"; cmd[3]->args[2] = NULL; 
}

void launch_job(void){
	printCommand();
	execvp(cmd[pointer_cmd]->args[0], cmd[pointer_cmd]->args);
	printf("error in executing process");
	exit(EXIT_FAILURE); 
}

void signalHandler(int signal_number, siginfo_t* si, void* context){
	switch(signal_number){
		case SIGCHLD:
			printf("Finishing job");
			int pid;
			pid = waitpid(WAIT_ANY, &status, WUNTRACED);
			jobs_completed++; 
			cmd[pointer_cmd]->complete = 1; //delete them from list
			cmd[pointer_cmd]->bg = 0; 
			break;
		case SIGSTOP: 
			printf("Stopping command"); printCommand(); 
			cmd[pointer_cmd]->bg = 1; 
			break;
		case SIGCONT: 
			printf("Continuing command");
			cmd[pointer_cmd]->bg = 0;
			launch_job();  /** Need to start the job? **/
			break;
		case SIGINT: 
			printf("Terminating command"); printCommand(); 
			cmd[pointer_cmd]->complete = 1; //not done but completed 
			cmd[pointer_cmd]->bg = 0; 
			break;
		case SIGTTIN: 
			break;
		case SIGTTOU: 
			break;
		default: 
			printf("Signal %d not supported", signal_number);
	}
}


//more complicated maneuver
void findJob(void){
	int rounds = 0;

	for(; pointer_cmd < 4 && 
		  cmd[pointer_cmd]->complete != 0 && 
		  cmd[pointer_cmd]->bg != 1 && rounds != 2; pointer_cmd++){
		if(pointer_cmd == 3){
			pointer_cmd = 0; 
			rounds++; 
		}
	}
}
//prints the current command
void printCommand(void){
	printf("Command: ");
	int i; 
	for(; cmd[pointer_cmd]->args[i] != NULL; i++){
		printf("%s ", cmd[pointer_cmd]->args[i]);
	}
	printf("with pid %d\n", cmd[pointer_cmd]->pid);
}

