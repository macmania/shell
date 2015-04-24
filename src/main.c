#include "Shell.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {
	init();
	printPrompt();
	firstJob = malloc(sizeof(job*));
	*firstJob = malloc(sizeof(job)); 
	while(TRUE) {
		char* cmdLine;
		job* j = malloc(sizeof(job));
		
		printf("shell: ");
		cmdLine = readCmdLine(); 
		
		if(isCmdEmpty(cmdLine)){
		  continue;
		}

		//cmd = parse(cmdLine);
		parseCommand(cmdLine, j); //takes the command and saves them to cmdType
		
		if(j->first_process == NULL){
			j->first_process = malloc(sizeof(process)); 
			parse(&(j->first_process), cmdLine);
		}
		
		
		if(j->first_process != NULL && isBltInCmd(j->first_process)){ 
		  	execBltInCmd(j->first_process); //stop, etc.
		}
		else {
			addJob(firstJob, j);
			launchJob(*firstJob);
		}
	  }
	  
	printf("\n");
}
