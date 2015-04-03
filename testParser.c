#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "parser.h"


char* getLine(void);
int main(int argc, char** arg) {
  // char* sampleChar;
  // char str[256];
  // while(1){
  //       sampleChar = getLine();

  //       printf("You entered %s, %lu, %c\n", sampleChar,  strlen(sampleChar), sampleChar[0]);
  // }

  char* command = "       echo input1 < a.out > output1 | something";
  int length = strlen(command)+1;
  char strPtr[strlen(command)+1];//[length];// = strdup(some);
  
  strncpy(strPtr, command, length);
  strPtr[strlen(command)]='\0';
  
  char* copy = strdup(strPtr);
  char* delim = "&><|";
  char* cmdPtr = strtok(strPtr, delim); 
  char cmdType; 
  char *temp, *prev=malloc(sizeof(char));

  //printf("%s\n", cmdPtr);

  while(cmdPtr){
    cmdType = copy[cmdPtr-strPtr+strlen(cmdPtr)];
   
    //printf("%lu,%lu\n",cmdPtr-strPtr ,cmdPtr-strPtr+strlen(cmdPtr)-1);
  //  printf("%s, %s\n", &copy[cmdPtr-strPtr], copy);
    printf("%lu, %lu\n", cmdPtr-strPtr, strlen(cmdPtr));
    memcpy(prev, &copy[cmdPtr-strPtr], strlen(cmdPtr));
    //printf("%s %c \n",prev, cmdType);
    printf("Something: %s\n", prev);
    memset(&prev[0], 0, strlen(prev));
    printf("Something: %s\n", prev);
    //printf("\n%s", )
    //printf("%s\t",cmdPtr);//, copy[cmdPtr-strPtr+strlen(cmdPtr)]);
    //printf("%d",cmdPtr-strPtr+strlen(cmdPtr));
    //printf("%c\n", copy[cmdPtr-strPtr+strlen(cmdPtr)]);
    
    //printf("%s", cmdPtr);
    
    // if(cmdType != '\0'){
    //   printf("%s",cmdPtr);
    //   prev = &copy[cmdPtr-strPtr+strlen(cmdPtr)+1]
    //   printf("%s", prev);
    //   temp = strtok(prev, " ");
    //   if(temp){
    //     printf("2 things: %s %s\n", prev, temp);
    //   }
    //   else {
    //     printf("%s\n", prev); 
    //   }
    // }
    cmdPtr = strtok(NULL, delim);
    //printf("%s\n\n", cmdPtr);


  }

  free(prev);

  return 0;
}

//returns the line in stdin
char* getLine(void){
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
