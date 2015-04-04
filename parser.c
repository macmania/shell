#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include <sys/stat.h>
#include <ctype.h> 
void init_info(struct parseInfo *p){
  p->command = malloc(sizeof(char)); 
  p->argVarNum = 0; //unitialize for now
}


//parses each of the tokens given the command
//still need to test with all of the base cases
void parse_command(char *command, commandType *comm){
  char *cmdTok, *delimeters, *cpyPtr, *prevTempCmdTok;
  char tempCmdTok[strlen(command)+1];
  int lenCommandStr=strlen(command)+1, locCmdType;
  char commandArrCopy[lenCommandStr];
  char commandType; 


  /*Temporary variables until determined*/
  int isPipe = -1; //temporary variable to ensure that the out command type is not valid once
                  //in pipe 

  delimeters = "&><|";
  strncpy(commandArrCopy, command, lenCommandStr);
  commandArrCopy[strlen(command)] = '\0';
  cpyPtr = strdup(commandArrCopy);
  cmdTok = strtok(commandArrCopy, delimeters);


  //Sample command: 
  /***** input < echo > output **/
  /**** --- To-do: need to improve the variable names --- ***/
  while(cmdTok){
    locCmdType = cmdTok - commandArrCopy + strlen(cmdTok);
    commandType = cpyPtr[locCmdType];
    
    switch(commandType) {
      
      case '<': //input file to the next command
        strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)); 
        prevTempCmdTok = trimWhiteSpaces(tempCmdTok); 

        if(!is_proper_file(prevTempCmdTok)){
          print_error(UNKNOWN_CMD); //may need to raise some sort of error or so
          return;
        }
        if(!is_file(prevTempCmdTok)){
          print_error(FILE_NAME_NOT_FOUND);
          return;
        }

        strncpy(comm->inFile, prevTempCmdTok, strlen(prevTempCmdTok));
        comm->isInFile=1;
       // printf("%s %s\n", prevTempCmdTok, nextCmdTok);
        break; 

      case '>': //save output to output file, signals the end of the command
        strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)); //---such as this---
        cmdTok = strtok(NULL, delimeters);
        if(!cmdTok){
          print_error(NO_FILE_ENTERED);
        }
        strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)); //---such as this---
        prevTempCmdTok = trimWhiteSpaces(tempCmdTok); 

        if(!is_proper_file(prevTempCmdTok)){
          print_error(UNKNOWN_CMD);
          return;
        }
        comm->isOutFile=1;
        strncpy(comm->outFile, prevTempCmdTok, strlen(prevTempCmdTok));
        break;

      case '|': //need more information here or guidance at the very the least
        //call an outside function here to set up comm->CmdArray that will save 
        
        printf("%lu", strlen(command) - (cmdTok - commandArrCopy));
        strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(command) - (cmdTok - commandArrCopy)); //---such as this---
        printf("%s", tempCmdTok);
        set_pipes(tempCmdTok, comm);

        //changes the location of the command string token
        int numPipes = comm->numPipes; 
        for(;numPipes > 0; numPipes--, cmdTok = strtok(NULL, delimeters));
        break;

      case '&':
        comm->isBackground = 1;
        break;
    }
    
    cmdTok = strtok(NULL, delimeters);
    memset(&tempCmdTok[0], 0, strlen(tempCmdTok));
  }

}

//sets the pipes with the appropriate str token. 
void set_pipes(char command[], commandType *cmdType){
  /** echo | a.out"  **/
printf("HERERE");
  char *pipeDelim, *tok, *cpyArr, *tempCmdTok;
  char cmdArrCpy[strlen(command)];
  int pipeLocation;

  pipeDelim="|";
  tok = strtok(command, pipeDelim);
  strncpy(cmdArrCpy, command, strlen(command));
  cpyArr = strdup(cmdArrCpy);

  while(tok){
    
    strncpy(tempCmdTok, &cpyArr[tok-cmdArrCpy], strlen(tok)); 
    cmdType->CmdArray[cmdType->numPipes] = *parse(tempCmdTok);
    cmdType->numPipes++; 
    
    tok = strtok(NULL, pipeDelim);
    memset(&tempCmdTok[0], 0, strlen(tempCmdTok)); //reset the temp command token
  }


}




char* trimWhiteSpaces(char* str){
  char* end; 

  while(isspace(*str)) str++; 

  if(*str == '\0') return str; 

  end = str + strlen(str) - 1; 
  while(end > str && isspace(*end)) end--; 

  *(end+1) = 0; 

  return str; 
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

//goes through each character
int is_proper_file(char* fileName){
  for(;*fileName;fileName++){
    if(isspace(*fileName)) return 0;
  }
  return 1;
}

/*Need to improve this error message more thoroughly**/
void print_error(enum error_msg_parse msg) {
  switch(msg){
    case FILE_NAME_NOT_FOUND: 
      printf("File or Directory not found\n"); 
      break;
    case UNKNOWN_CMD: 
      printf("Command cannot be processed succesfully\n"); 
      break;
    case NO_FILE_ENTERED:
      printf("Please enter a file name");
      break;
  }
}



//tests the methods, not yet adept in using test cases
//**
int main(void){
  struct parseInfo* info; 
  commandType* type; 

  char* command = "input112 < echo > output";
  char* commandPipe = "ls -l | grep \"Apr\"";



  // printf("hello");

  // info = parse("cd\n"); 
  // printf("%s", info->command); 

  // int i; 
  // for(i = 0; i < info->argVarNum; i++){
  //   printf("%s\n", info->ArgVarList[i]); 
  // }

  // printf("%s %d", getenv("HOME"), chdir("../"));
  // system("ls");
  type = malloc(sizeof(commandType));
  //parse_command(command, type);
  parse_command(commandPipe, type);
  //printf("%s %s\n", type->inFile, type->outFile);

  printf("%d", type->numPipes);


  return 0; 
}
/**/