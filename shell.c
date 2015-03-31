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
#include <unistd.h>//#include "parser.h"
// #include "parser.h"

int main (int argc, char** argv) {
  printPrompt();
  //add a list that adds commands to the list
  while(1) {
    int childPid; 
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


void waitPid(int pid) {

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

void execBuiltInCmd(struct parseInfo* cmd) {
  
}

void executeCommand(struct parseInfo* cmd) {

}

bool isBackgroundJob(struct parseInfo* cmd) {
  return false; 
}

bool isBuiltCommand(struct parseInfo* command){
  if(command){
    return false;
  }
  else {
    return true; //temporary
  }
}

bool isCmdEmpty(char* cmd){
  if(cmd == NULL || cmd[0] == '\0' || cmd[0] == '\n'){
    return true; 
  }
  return false; 
}