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
  printPrompt();
  //add a list that adds commands to the list
  while(1) {
    int childPid; 
    char* cmdLine;
    parseInfo* cmd; 
    commandType* cmdType; 

    cmdLine = readCmdLine(); //tokenizes the commands
    //perhaps record the commands typed in a document
    if(ifCmdEmpty(cmdLine)){
      continue; 
    }
        
    parse(cmdLine); 

    if (isBuiltCommand(cmd)){
      execBuiltInCmd(cmd); //stop, etc.
    } else {
        childPid = fork();
        if(childPid == 0) {
          executeCommand(cmd); //calls execvp
        } else {
          if(isBackgroundJob(cmd)) {
            //record in a list of background jobs
          } else {
            waitPid(childPid);
          }
        }
    }
  }
}

void printPrompt(){
  printf("Welcome to Jojo's small scale shell");
}

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

    *line = '\0';
    return linep;
  }
}


bool isBuiltCommand(char* command){
  if(command){
    return false;
  }
  else {
    return true; //temporary
  }
}

bool isCmdEmpty(char* cmd){
  if(cmdLine == NULL || cmdLine[0] == '\0' || cmdLine[0] == '\n'){
    return true; 
  }
  return false; 
}