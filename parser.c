#include <stdio.h>
#include "parser.h"
#include <string.h>
void init_info(parseInfo *p){
  p = malloc(sizeof(parseInfo));
}

//parses each of the tokens given the command
void parse_command(char *command, commandType *comm){
  
}

parseInfo *parse (char* cmdLine){
  char* tokCmds, tmpCmd; 
  parseInfo* result; 

  strcpy(s, cmdLine);
  init_info(result); 

  if(result == NULL){
    fprintf(stderr, "cannot initialize memory block"); 
    return NULL; 
  }

  tokCmds = strtok(tmpCmd, " "); 

  result->command = tokCmds; //maybe add a method to further wrap the initialization process better


  //still need to do the piping cmd
  while((tokCmds = strtok(NULL, " ")){
    //to-do, need to add the variation of the command 
  }

  return result; 

}

void print_info(parseInfo *info){
	printf("Command: %s", info->command);  //temp, 
}

void free_info(parseInfo *info){
	printf("freeing memory block"); 
	free(info); 
}

//tests the methods, not yet adept in using test cases
int main(void){

}