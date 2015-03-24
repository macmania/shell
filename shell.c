//Small project as a shell
/***
  Part 1: build a small scale shell that handles all of the commands just like a
  unix like shell (be able to handle fork and parent stuff)
  Part 2: handle multi-threading
**/
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h> //fork
#include "shell.h" //to-do need to put this in a directory
 #include <unistd.h>

int main (int argc, char** argv) {
  printf("hello");
  while(1) {

    int childPid;
    char* command;

    printPrompt(); //prints basic prompt

    command = readCmdLine(); //tokenizes the commands
    //perhaps record the commands typed in a document

    if (isBuiltCommand(command)){
      execBuiltInCmd(command); //stop, etc.
    } else {
        childPid = fork();
        if(childPid == 0) {
          executeCommand(command); //calls execvp
        } else {
          if(isBackgroundJob(command)) {
            //record in a list of background jobs
          } else {
            waitPid(childPid);
          }
        }
    }
  }
}

void printPrompt(){
  printf("Hello");
}
