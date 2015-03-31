//#ifndef Shell_h
//#define Shell_h
#include "parser.h"

typedef int bool;
#define true 1
#define false 0


void printPrompt();
char* readCmdLine();
bool isBuiltCommand();
void execBuiltInCmd(struct parseInfo*);
void executeCommand(struct parseInfo*);
bool isBackgroundJob(struct parseInfo*); //goes through a linked list to see if the command is paused
void waitPid(int);
bool isCmdEmpty(char*); 
//#endif
