#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

void init_info(struct parseInfo *p){
  p->command = malloc(sizeof(char)); 
  p->argVarNum = 0; //unitialize for now
}

//parses each of the tokens given the command
void parse_command(char *command, commandType *comm){

}

struct parseInfo* parse (char* cmdLine){
  char *tokCmds; 
  char tmpCmd[strlen(cmdLine)+1]; 

  struct parseInfo* result; 
  result = malloc(sizeof(struct parseInfo));  
  strcpy(tmpCmd, cmdLine);
  init_info(result); 

  if(result == NULL){
    fprintf(stderr, "cannot initialize memory block"); 
    return NULL; 
  }

  tokCmds = strtok(tmpCmd, " "); 
  memcpy(result->command, tokCmds, strlen(tokCmds)); //maybe add a method to further wrap the initialization process better


  // //still need to do the piping cmd
  int i = 0; 
  while((tokCmds = strtok(NULL, " "))){
    result->ArgVarList[i] = tokCmds; 
    i++; 
  }

  result->argVarNum = i; 
  
  return result; 
}

void print_info(struct parseInfo *info){
	if(info == NULL) {
    return;
  }

  printf("%s ", info->command); 

  int i; 
  for(i = 0; i < info->argVarNum; i++){
    printf("%s ", info->ArgVarList[i]); 
  }
}

void free_info(struct parseInfo *info){
	printf("freeing memory block"); 
	free(info); 
}

//tests the methods, not yet adept in using test cases
/**int main(void){
  struct parseInfo* info; 
  // printf("hello");

  info = parse("cd hello | pipe | hello eh"); 
  printf("%s", info->command); 

  int i; 
  for(i = 0; i < info->argVarNum; i++){
    printf("%s\n", info->ArgVarList[i]); 
  }

  return 0; 
}**/