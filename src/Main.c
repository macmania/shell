#include "Shell.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {
	init();
	printPrompt();
	firstJob = malloc(sizeof(job));
	while(TRUE) {
		char* cmdLine;
		struct parseInfo* cmd;
		commandType* cmdType;
		cmdType = malloc(sizeof(commandType));
		
		printf("shell: ");
		cmdLine = readCmdLine(); 
		
		if(isCmdEmpty(cmdLine)){
		  continue;
		}

		cmd = parse(cmdLine);
		parseCommand(cmdLine, cmdType); //takes the command and saves them to cmdType
		
		if (isBltInCmd(cmd)){ //Limitations, built in commands cannot handle &, | or so 
								//for now
		  execBltInCmd(cmd); //stop, etc.
		}
		else {
			job* j = malloc(sizeof(job));
			j = readyJob(cmdLine, cmd, cmdType, j);
			launchJob(j);
		}
	  }
	  
	
	printf("\n");
}