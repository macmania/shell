#include <stdio.h>
#include "parser.h"

void init_info(parseInfo *p){
  p = (parseInfo*)malloc(sizeof(parseInfo));
}

void parse_command(char *command, struct commandType *comm){

}

parseInfo *parse (char* cmdLine){
  foreach cmd in cmdline{
    if(cmd == command) {
      parse_command(cmd, type);
    }
  }
}

void print_info(parseInfo *info){

}

void free_info(parseInfo *info){

}
