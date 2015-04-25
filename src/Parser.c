#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "Parser.h"
#include <sys/stat.h>
#include <ctype.h> 

void initProcess(process* p){
  //p = malloc(sizeof(process));
  p->command = malloc(sizeof(char)); 
  p->argVarNum = 0;
  p->next = NULL; 
}

//parses each of the tokens given the command
//still need to test with all of the base cases
void parseCommand(char *command, job *j){
    if(j == NULL){
  	  printf("Error, command type pointer not initialized");
  	  return;
    }
  	
    char *cmdTok, *delimeters, *cpyPtr, *prevTempCmdTok;
    char tempCmdTok[strlen(command)+1];
    int lenCommandStr=strlen(command)+1, locCmdType;
    char commandArrCopy[lenCommandStr];
    char cmdType; 

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
      cmdType = cpyPtr[locCmdType];

      switch(cmdType) { 
    	  case DIRECT_IN: //redirect input file to the next command
          strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)+1); 
          tempCmdTok[strlen(cmdTok)] = '\0';
          prevTempCmdTok = trimWhiteSpaces(tempCmdTok); 

          
          if(!isProperFile(prevTempCmdTok)){
            printError(UNKNOWN_CMD); //may need to raise some sort of error or so
            return;
          }
          
          if(!isFile(prevTempCmdTok)){
            printError(FILE_NAME_NOT_FOUND);
            return;
          }

          strncpy(j->inFile, prevTempCmdTok, strlen(prevTempCmdTok));
          j->isInFile = 1; //temporary
          break; 

        case DIRECT_OUT: //redirect output to specified file, signals the end of the command
          strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)+1); //---such as this---, added +1
          
          cmdTok = strtok(NULL, delimeters);
          if(!cmdTok){
            printError(NO_FILE_ENTERED);
            return;
          }
          strncpy(tempCmdTok, &cpyPtr[cmdTok-commandArrCopy], strlen(cmdTok)+1); //---such as this---, added+ 1
          tempCmdTok[strlen(cmdTok)] = '\0';
          prevTempCmdTok = trimWhiteSpaces(tempCmdTok);

          if(!isProperFile(prevTempCmdTok)){
            printError(UNKNOWN_CMD);
            return;
          }

          j->isOutFile=1;
          strncpy(j->outFile, prevTempCmdTok, strlen(prevTempCmdTok));
          break;

        
        case PIPE: //need more information here or guidance at the very the least
          //call an outside function here to set up comm->CmdArray that will save 
          cmdTok = strtok(NULL, delimeters);
          strncpy(tempCmdTok, &cpyPtr[cmdTok - commandArrCopy], strlen(cmdTok)+1); 
          tempCmdTok[strlen(cmdTok)] = '\0';
          if(tempCmdTok){
            //may need to change some stuff or so 
            printError(UNKNOWN_CMD);
            return;
          }
          prevTempCmdTok = trimWhiteSpaces(tempCmdTok);
         /** j->CmdArray[comm->numPipes]**/ 
          process* p = malloc(sizeof(process));
          parse(&p, prevTempCmdTok);
          addProcessEnd(&(j->first_process), p); 
          j->numPipes++;

          break;

        case BACKGROUND_INPUT:
          j->status = BACKGROUND;
          break;
      }

      cmdTok = strtok(NULL, delimeters);
      memset(&tempCmdTok[0], 0, strlen(tempCmdTok));
    }

    cpyPtr = trimWhiteSpaces(cpyPtr); //time-consuming 
    if(cpyPtr[strlen(cpyPtr)-1] == BACKGROUND_INPUT){
      j->status = FOREGROUND;
    }
    if(j->commandType != '\0') 
        j->commandType = cmdType;
    else{
    	  j->commandType = NORMAL_CMD;
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

void parse(process** p, char* cmdLine){
  char *tokCmds; 
  char tmpCmd[strlen(cmdLine)+1]; 

  strncpy(tmpCmd, cmdLine, strlen(cmdLine) +1);
  tmpCmd[strlen(cmdLine) + 1] = '\0';
  initProcess(*p); 

  if(p == NULL){
    fprintf(stderr, "cannot initialize memory block"); 
   // return NULL; 
  }


  int next = 0, past = 0, i=0; 

  for(; tmpCmd[next] != ' ' && tmpCmd[next] != '\0' && next < (strlen(cmdLine)); next++);
  
  memcpy((*p)->command, &tmpCmd[past], next-past);
 
  if(isFile((*p)->command)){ //this still needs to be tested much further
	  past = next;
	  sleep(1);
	  for(; tmpCmd[next] != ' ' && tmpCmd[next] != DIRECT_IN; next++);
  }
  else{
	  //addProcess(p)
	  (*p)->ArgVarList[i] = malloc(sizeof(char)); 
	  memcpy((*p)->ArgVarList[i], (*p)->command, next-past); 
	  i++;
	  past = next;
	  next++;
	 
	  for(; next < strlen(cmdLine); next++){
  		if(tmpCmd[next] == ' '){
  			(*p)->ArgVarList[i] = malloc(sizeof(char));
  			memcpy((*p)->ArgVarList[i], &tmpCmd[past], next-past);
  			past = next; 
  			i++;
  		} 
	  }
	
	  if(past != next && tmpCmd[past] != '\0'){
		  (*p)->ArgVarList[i] = malloc(sizeof(char));
		  memcpy((*p)->ArgVarList[i], trimWhiteSpaces(&tmpCmd[past]), next-past);
		  i++;
	  }
	  (*p)->ArgVarList[i] = NULL;
	  (*p)->argVarNum = i;
  }
  //return result; 
}

//Prints each process' command
void printProcess(struct process *p){
  if(p == NULL) {
    return;
  }

  printf("command: %s ", p->command); 

  process* temp;
  int i; 
  for(i = 0; i < p->argVarNum; i++){
    printf("%s ", p->ArgVarList[i]); 
  }
}

int isFile(char* fileName){
  struct stat buffer;
  return (stat(fileName, &buffer) == 0);
}

//goes through each character
int isProperFile(char* fileName){
  for(;*fileName;fileName++){
    if(isspace(*fileName)) return 0;
  }
  return 1;
}

/*Need to improve this error message more thoroughly**/
void printError(enum error_msg_parse msg) {
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
