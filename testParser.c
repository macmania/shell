#include <stdio.h>
#include <stdlib.h>
//#include "parser.h"


char* getLine(void);
int main(int argc, char** arg) {
  char* sampleChar;
  char str[256];
  while(1){
        sampleChar = getLine();
        printf("You entered %s, %lu\n", sampleChar, sizeof(sampleChar));
  }
  return 0;
}

//returns the line in stdin
char* getLine(void){
  char* line = malloc(2), *linep = line;
  size_t lenMax = 2, len = lenMax;
  int c;

  if(line == NULL){
    return NULL;
  }
  else {
    while((c = fgetc(stdin)) != EOF){
      printf("line: %s, address: %p, sizeof: %ld\n",linep, &linep, sizeof(linep));

      if(--len == 0){ //resize the size
        printf("\nHere\n");
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


      printf("\nline pointer: %c, %p\n", *line--, &line);

      if((*line++ = c) == '\n'){
        break;
      }
    }

    *line = '\0';
    return linep;
  }

}
