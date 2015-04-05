#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include <sys/stat.h>
#include <ctype.h> 
void init_info(struct parseInfo *p){
  p->command = malloc(sizeof(char)); 
  p->argVarNum = 0;
}

//parses each of the tokens given the command
//still need to test with all of the base cases
void parse_command(char *command, commandType *comm){
  char *cmdTok, *delimeters, *cpyPtr;
  char *prevTempCmdTok;
  char tempCmdTok[strlen(command)+1];
  int lenCommandStr=strlen(command)+1, locCmdType;
  char commandArrCopy[lenCommandStr];
  char commandType; 

  delimeters = "&><|";
  strncpy(commandArrCopy, command, lenCommandStr);
  commandArrCopy[strlen(command)+1] = '\0';
  cpyPtr = strdup(commandArrCopy);
  cmdTok = strtok(commandArrCopy, delimeters);

  //Sample command: 
  /***** input < echo > output **/
  /**** --- To-do: need to improve the variable names --- ***/
  while(cmdTok){
    locCmdType = cmdTok - commandArrCopy + strlen(cmdTok);
    commandType = cpyPtr[locCmdType];

    switch(commandType) {
      
      case '<': //redirect input file to the next command
        strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)+1); 
        tempCmdTok[strlen(cmdTok)] = '\0';
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
        break; 

      case '>': //redirect output to specified file, signals the end of the command
        strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)+1); //---such as this---, added +1
        
        cmdTok = strtok(NULL, delimeters);
        if(!cmdTok){
          print_error(NO_FILE_ENTERED);
          return;
        }
        strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)+1); //---such as this---, added+ 1
        tempCmdTok[strlen(cmdTok)] = '\0';
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
        cmdTok = strtok(NULL, delimeters);
        strncpy(tempCmdTok, &cpyPtr[cmdTok - commandArrCopy], strlen(cmdTok)+1); 
        tempCmdTok[strlen(cmdTok)] = '\0';
        prevTempCmdTok = trimWhiteSpaces(tempCmdTok);
        comm->CmdArray[comm->numPipes] = *parse(prevTempCmdTok);
        comm->numPipes++;

        break;

      case '&':
        comm->isBackground = 1;
        break;
    }

    cmdTok = strtok(NULL, delimeters);
    memset(&tempCmdTok[0], 0, strlen(tempCmdTok));
  }

  cpyPtr = trimWhiteSpaces(cpyPtr); //time-consuming 
  if(cpyPtr[strlen(cpyPtr)-1] == '&'){
    comm->isBackground = 1;
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
  tmpCmd[strlen(cmdLine)] = '\0';
  init_info(result); 

  if(result == NULL){
    fprintf(stderr, "cannot initialize memory block"); 
    return NULL; 
  }


  int next = 0, past = 0, i=0; 

  for(; tmpCmd[next] != ' '; next++);
  memcpy(result->command, &tmpCmd[past], next-past);
  past = next;
  next++;

  for(; next < strlen(cmdLine); next++){
    if(tmpCmd[next] == ' '){
      result->ArgVarList[i] = malloc(sizeof(char));
      memcpy(result->ArgVarList[i], &tmpCmd[past], next-past);
      past = next; 
      i++;
    } 
  }

  if(past != next){
    result->ArgVarList[i] = malloc(sizeof(char));
    memcpy(result->ArgVarList[i], trimWhiteSpaces(&tmpCmd[past]), next-past);
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
  commandType* type, *type2; 

  char* command = "input112 < echo > output &   ";
  char* commandPipe = "ls -l | grep \"Apr\" > output &";

  type = malloc(sizeof(commandType));
  type2 = malloc(sizeof(commandType));
  parse_command(command, type2);
  parse_command(commandPipe, type);



   int i; 
  for(i=0; i < type->numPipes; i++){
   
    print_info(&type->CmdArray[i]);
    //print_info(&type2->CmdArray[i]);
    printf("------\n------");
  }



  //printf("\n\n%s %d\n", type->outFile, type->isBackground);

  free(type);
  free(type2);

  return 0; 
}
/**/