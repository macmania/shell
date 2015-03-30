#include <stdio.h>
#include "parser.h"

void init_info(parseInfo *p){
  p = malloc(sizeof(parseInfo));
}

void parse_command(char *command, commandType *comm){

}

parseInfo *parse (char* cmdLine){

}

void print_info(parseInfo *info){
	printf("Command: %s", info->command);  //temp, 
}

void free_info(parseInfo *info){
	printf("freeing memory block"); 
	free(info); 
}
