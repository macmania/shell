//temporary holder for shell.c

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(void) {
	pid_t childPid; 
	int childStatus; 
	char *args[2];

	args[0] = "ls"; args[1] = "-l"; //args[2] = NULL;
	childPid = fork(); 

	if(childPid == 0) {
		execv(args[0], args);
		printf("unknown command\n");
		exit(0);
	}	
	else{
		pid_t tpid;
		do{
			tpid = wait(&childStatus);
			printf("tpid: %d child_pid: %d child_status: %d\n", tpid, childPid, childStatus);
			if(tpid != childPid){
				printf("Here");
				kill(tpid, SIGINT);
			}
		}while(tpid != childPid);
		
		return childStatus;
	}
	

}


