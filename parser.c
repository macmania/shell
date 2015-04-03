#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include <sys/stat.h>

void init_info(struct parseInfo *p){
  p->command = malloc(sizeof(char)); 
  p->argVarNum = 0; //unitialize for now
}


//parses each of the tokens given the command
//still need to test with all of the base cases
void parse_command(char *command, commandType *comm){
  char *cmdTok, *delimeters, *cpyPtr, *tempCmdTok, *prevTempCmdTok; 
  int lenCommandStr, locCmdType, numTokens=0;
  char commandArrCopy[lenCommandStr];
  char commandType; 

  delimeters = "><|&";
  lenCommandStr = strlen(command)+1;
  strncpy(commandArrCopy, command, lenCommandStr);
  cpyPtr = strdup(commandArrCopy);
  cmdTok = strtok(commandArrCopy, delimeters);
  numTokens = numTokens + 1;
  comm = malloc(sizeof(commandType));
  
  printf("%s\t%s\t%s\t%s", command, cpyPtr, commandArrCopy,cmdTok );

  while(cmdTok){
    locCmdType = cmdTok - commandArrCopy + strlen(cmdTok);
    commandType = cpyPtr[locCmdType];

    switch(commandType) {
      case '>':
       //   tempCmdTok = itrim(cmdTok); 
          prevTempCmdTok = strtok(tempCmdTok, " ");
          tempCmdTok=strtok(NULL, " ");

          if(tempCmdTok) { //is not null then prevTempCmdTok better be a command
            if(is_file(tempCmdTok)){

            }
            else {

            }
          }
        break;
      case '<': 
        break; 
      case '|':
        break;
      case '&':
        comm->isBackground = 1;
        break;
    }
    
    cmdTok = strtok(NULL, delimeters);
    numTokens++;

  }


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

  tokCmds = strtok(tmpCmd, " \n"); 
  memcpy(result->command, tokCmds, strlen(tokCmds)); //maybe add a method to further wrap the initialization process better


  // //still need to do the piping cmd
  int i = 0; 
  while((tokCmds = strtok(NULL, " \n"))){
    result->ArgVarList[i] = malloc(sizeof(char));
    strncpy(result->ArgVarList[i], tokCmds, strlen(tokCmds));
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
	int i; 
  for(i = 0; i < info->argVarNum; i++){
    free(info->ArgVarList[i]);
  }
  free(info); 

}

int is_file(char* fileName){
  struct stat buffer; 
  return (stat(fileName, &buffer) == 0);
}

//tests the methods, not yet adept in using test cases
/**
int main(void){
  struct parseInfo* info; 
  commandType* type; 
  char* command = "echo input1 < a.out > output1 | something";
  // printf("hello");

  // info = parse("cd\n"); 
  // printf("%s", info->command); 

  // int i; 
  // for(i = 0; i < info->argVarNum; i++){
  //   printf("%s\n", info->ArgVarList[i]); 
  // }

  // printf("%s %d", getenv("HOME"), chdir("../"));
  // system("ls");

  parse_command(command, type);


  return 0; 
}
**/