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
  char *cmdTok, *delimeters, *cpyPtr, *prevTempCmdTok = malloc(sizeof(char));
  char tempCmdTok[strlen(command)+1];
  int lenCommandStr=strlen(command)+1, locCmdType;
  char commandArrCopy[lenCommandStr];
  char commandType; 


  /*Temporary variables until determined*/
  int isPipe = -1; //temporary variable to ensure that the out command type is not valid once
                  //in pipe 

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
    printf("%s %c\t", cmdTok, commandType);
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
       // printf("%s %s\n", prevTempCmdTok, nextCmdTok);
        break; 

      case '>': //redirect output to specified file, signals the end of the command
        printf("HERE before %s \n", cmdTok);
        strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)+1); //---such as this---, added +1
        
        cmdTok = strtok(NULL, delimeters);
        if(!cmdTok){
          print_error(NO_FILE_ENTERED);
        }
        strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)+1); //---such as this---, added+ 1
        tempCmdTok[strlen(cmdTok)] = '\0';
        prevTempCmdTok = trimWhiteSpaces(tempCmdTok); 

        if(!is_proper_file(prevTempCmdTok)){
          print_error(UNKNOWN_CMD);
          return;
        }
        printf("HERE %s \n", cmdTok);
        comm->isOutFile=1;
        strncpy(comm->outFile, prevTempCmdTok, strlen(prevTempCmdTok));
        break;

      case '|': //need more information here or guidance at the very the least
        //call an outside function here to set up comm->CmdArray that will save 
        

        //Error here
        cmdTok = strtok(NULL, delimeters);
        strncpy(tempCmdTok, &cpyPtr[cmdTok - commandArrCopy], strlen(cmdTok)+1); 
        tempCmdTok[strlen(cmdTok)] = '\0';
        strncpy(prevTempCmdTok, trimWhiteSpaces(tempCmdTok), strlen(tempCmdTok)+1);
        printf("%d\n", comm->numPipes);
        comm->CmdArray[comm->numPipes] = *parse(prevTempCmdTok);
        comm->numPipes++;


        //printf("%s %s %s\n", cmdTok, tempCmdTok, prevTempCmdTok);
        
       // printf("%s %s %s", cmdTok, tempCmdTok, prevTempCmdTok);
        // strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(command) - (locCmdType-strlen(cmdTok))+1); //---such as this---
        // printf("before temp: %s cmdTok: %s\n", tempCmdTok, cmdTok);
        // set_pipes(tempCmdTok, comm);
        // printf("after temp: %s cmdTok: %s\n", tempCmdTok, cmdTok);
        // //changes the location of the command string token
        
        // int numPipes = comm->numPipes; 
        // printf("%d\n", numPipes);
        // for(;numPipes > 0; numPipes--){
        //    printf("%s\n", cmdTok);
        //    cmdTok = strtok(NULL, delimeters);
        //    printf("%s\n", cmdTok);
        // }
        // printf("after iter temp: %s cmdTok: %s\n", tempCmdTok, cmdTok);
        isPipe = 1; 
      
        break;

      case '&':
        comm->isBackground = 1;
        break;
    }

    // if(isPipe == 1){
    //   int numPipes = comm->numPipes+1; 
    //   printf("%d", numPipes);
    //   for(;numPipes > 0; numPipes--){
    //      //printf("HERE");
    //      //cmdTok = strtok(NULL, delimeters);
    //      printf("%s\n", cmdTok);
    //   }
    //   isPipe = 0;
    // }

    if(isPipe != 1)
      cmdTok = strtok(NULL, delimeters);
    else 
      isPipe = 0;
    memset(&tempCmdTok[0], 0, strlen(tempCmdTok));
  }

  cpyPtr = trimWhiteSpaces(cpyPtr); //time-consuming 
  if(cpyPtr[strlen(cpyPtr)-1] == '&'){
    comm->isBackground = 1;
  }

  free(prevTempCmdTok);
}

//sets the pipes with the appropriate string token command
void set_pipes(char cmd[], commandType *cmdType){
  /** echo | a.out"  **/
  
  char *pipeDelim, *tok, *cpyArr, *prevTmpCmdTok;
  char cmdArrCpy[strlen(cmd)+1];
  char tmpCmdTok[strlen(cmd+1)];
  int pipeLocation;

  pipeDelim="|&>";
  memcpy(cmdArrCpy, cmd, strlen(cmd));
  cmdArrCpy[strlen(cmd)] = '\0';
  cpyArr = strdup(cmdArrCpy);
  //printf("%s\n%s\n", cmd, cmdArrCpy);
  tok = strtok(cmdArrCpy, pipeDelim);
  memset(&tmpCmdTok[0], 0, strlen(tmpCmdTok));

  while(tok){
    pipeLocation = tok - cmdArrCpy + strlen(tok);
   
    

    strncpy(tmpCmdTok, &cpyArr[tok - cmdArrCpy], strlen(tok)+1); 
    tmpCmdTok[strlen(tok)] = '\0';
    cmdType->CmdArray[cmdType->numPipes] = *parse(tmpCmdTok);
    cmdType->numPipes++; 
    memset(&tmpCmdTok[0], 0, strlen(tmpCmdTok));
    printf("%c\nHelo",cpyArr[pipeLocation+strlen(tok)]);
    print_info(&cmdType->CmdArray[0]);

    //strlen(command) - (locCmdType-strlen(cmdTok))+1
    if(cpyArr[pipeLocation+strlen(tok)] == '>') { 
      tok = strtok(NULL, pipeDelim);
      printf("\nSample: %s \n",tok);
      strncpy(tmpCmdTok, &cpyArr[pipeLocation+strlen(tok)+1], strlen(tok)+1);
      prevTmpCmdTok = trimWhiteSpaces(tmpCmdTok);
      cmdType->isOutFile = 1; 
      strncpy(cmdType->outFile, prevTmpCmdTok, strlen(prevTmpCmdTok));
      printf("\nSSS: ---%s---\n", prevTmpCmdTok);
      tok = strtok(NULL, pipeDelim);
      
    
      if(tok != NULL){
        print_error(UNKNOWN_CMD);
        return;
      }

      break;
      
    }
    tok = strtok(NULL, pipeDelim);

  }


  cpyArr = trimWhiteSpaces(cpyArr); //time-consuming 
  if(cpyArr[strlen(cpyArr)-1] == '&'){
    cmdType->isBackground = 1;
  }
  //pipeLocation = tok - cmdArrCpy;
  //printf("Here1111, %s %d %c xx\n", &cpyArr[pipeLocation], pipeLocation, cpyArr[pipeLocation]);
  // if(cpyArr[pipeLocation] == '>'){
  //     printf("Here, %s\n", cpyArr);

  //     strncpy(tmpCmdTok, &cpyArr[pipeLocation+strlen(tok)], strlen(tok)+1); 
      
  //     printf("%s\n",  &cpyArr[pipeLocation]);
  //     tmpCmdTok[strlen(tok)] = '\0';
  //     prevTmpCmdTok = trimWhiteSpaces(tmpCmdTok);
  //     printf("%s %s", prevTmpCmdTok, tmpCmdTok);
  //     strncpy(cmdType->outFile, prevTmpCmdTok, strlen(prevTmpCmdTok));
  //     cmdType->isOutFile = 1;
  //     return;
  //   }
  //   if(cpyArr[pipeLocation] == '&'){
  //     cmdType->isBackground = 1;
  //     return;
  //   }
 
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
  commandType* type, *type2; 

  char* command = "input112 < echo > output &   ";
  char* commandPipe = "ls -l | grep \"Apr\" > output &";



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
  type2 = malloc(sizeof(commandType));
  //parse_command(command, type2);
  parse_command(commandPipe, type);

// printf("%s %s %d\n", type2->inFile, type2->outFile, type2->isBackground);

  //printf("%d", type->numPipes);



  int i; 
  for(i=0; i < type->numPipes; i++){
    printf("\n");
    print_info(&type->CmdArray[i]);
  }

  printf("\n%s %d\n", type->outFile, type->isBackground);

  free(type);
  free(type2);

  return 0; 
}
/**/