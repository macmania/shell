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
  char *cmdTok, *delimeters, *cpyPtr, *prevTempCmdTok, *nextCmdTok, tempCmdTok[strlen(command)+1];
  int lenCommandStr=strlen(command)+1, locCmdType, numTokens=0, isInFile_Dir=-1;
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
  numTokens = numTokens + 1;


  //Sample command: 
  /***** input < echo > output **/

  while(cmdTok){
    locCmdType = cmdTok - commandArrCopy + strlen(cmdTok);
    commandType = cpyPtr[locCmdType];
    
    memcpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)); 
    
    if(!(prevTempCmdTok = strtok(tempCmdTok, " "))){
      prevTempCmdTok = strdup(tempCmdTok);
    }
    nextCmdTok=strtok(NULL, " ");  
    
    if(nextCmdTok) { //is not null then prevTempCmdTok better be a command
      if(is_file(nextCmdTok)){
        isInFile_Dir=3; //11
      }
      else { //it's a string and needs to be saved somewhere
        isInFile_Dir=2; //2
      }
    }
    else{
      if(is_file(prevTempCmdTok)){
         isInFile_Dir=1; //means it is a file or directory
      }
      else{
        isInFile_Dir=0; //means it should be a normal command
      }
    } 
    //printf("%c\n", commandType);
    switch(commandType) {
      
      case '>':
        //if(isInFile){ //there is a file or directory that needs to be saved
        comm->isOutFile=1;
        comm->isInFile = 0;
        comm->isBackground=0;
        comm->commandType=DEPART_FILE;
        cmdTok = strtok(NULL, delimeters);
        strncpy(comm->outFile, cmdTok, strlen(cmdTok));
        memset(&(comm->inFile[0]), '\0', strlen(comm->inFile));
          //remove spaces in commTok; 
        //}
        break;
      case '<':
         printf("why??? %d %s %s", isInFile_Dir, prevTempCmdTok, nextCmdTok);

        if(isInFile_Dir==1){ //the very first token is a file
          strncpy(comm->inFile, prevTempCmdTok, strlen(cmdTok)); //im so confused here, need to rest
        }
        else if(isInFile_Dir==3){
          strncpy(comm->inFile, nextCmdTok, strlen(cmdTok)); //im so confused here, need to rest
        }

        comm->isInFile=1;
        comm->isOutFile = 0;
        comm->isBackground=0;
        comm->commandType=ENTER_FILE;
       // comm->outFile[0]=0;
       // memset(&(comm->outFile)[0], '\0', strlen(comm->outFile));
        //need to process the rest of the commands
        cmdTok = strtok(NULL, delimeters);
        if(cmdTok == NULL) {
         
          printf("\n%s %s\n", comm->inFile, comm->outFile);
          printf("hererere");
          return; //this is where the problem lies, need to be able
              /*----- temporary fix ---*/
        }

        break; 
      case '|': //need more information here or guidance at the very the least
        //call an outside function here to set up comm->CmdArray that will save 
        break;
      case '&':
        comm->isBackground = 1;
        comm->isInFile=0;
        comm->isOutFile=0;
        strncpy(comm->inFile, 0, strlen(comm->inFile));
        strncpy(comm->outFile, 0, strlen(comm->outFile));
        break;
      default: 
        cmdTok = strtok(NULL, delimeters);
        break;
    }
    
   // cmdTok = strtok(NULL, delimeters);
    memset(&tempCmdTok[0], 0, strlen(cmdTok));
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
//**
int main(void){
  struct parseInfo* info; 
  commandType* type; 

  char* command = "input1 < echo";
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
  parse_command(command, type);
  printf("%s %s\n", type->inFile, type->outFile);

  return 0; 
}
/**/