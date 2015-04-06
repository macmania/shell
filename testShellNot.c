//temporary holder for shell.c
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>


int main(void){

	pid_t childPid;
	int childStatus; 
	char* command = "ls";

	childPid = fork(); 

	if(childPid == 0){
		execv(command, "-l");

		printf("Unknown command\n");
		exit(EXIT_SUCCESS);
	}
	else {
		do {
			pid_t tpid = wait(&childStatus);
			if(tpid != childPid) 
				kill(tpid, SIGINT);
		} while(tpid != childPid);
	}

	return 0; 
}