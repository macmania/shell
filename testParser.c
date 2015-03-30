#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "parser.h"


char* getLine(void);
int main(int argc, char** arg) {
  char* sampleChar;
  char str[256];
  while(1){
        sampleChar = getLine();

        printf("You entered %s, %lu\n", sampleChar,  strlen(sampleChar));
  }
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
