//temporary holder for shell.c
//Handling signal interruption

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "/Users/jojofabe/Workspace/c/shell/Shell.h"

int main(void) {
	pid_t childPid; 
	int childStatus; 
	char *args[3][10];
	char *temp[2]; temp[0] = "ls"; temp[1]=NULL;

	args[0][0] = "ls"; args[0][1] = "-l"; args[0][2] = NULL;
	printf("%s\n%s\n%s\n", *(args[0]), args[0][1], args[0][2]);

	childPid = fork(); 

	if(childPid == 0) {
		execvp("ls", temp);
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


