/**
* Need to dynamically change the size of the arg var list in the future. 
*/
#include "JobControl.h"

void initProcess(process*);
void parseCommand(char*, job*);
void parse (process**, char*);
void printProcess(process*);
void freeInfo(process*);
void freeCmdType(job*);
void printError(enum error_msg_parse);
int  isFile(char* fileName); 
char* trimWhiteSpaces(char*);
int isProperFile(char*);
