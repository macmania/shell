//#ifndef Shell_h
//#define Shell_h

typedef int bool;
#define true 1
#define false 0


void printPrompt();
char* readCmdLine();
bool isBuiltCommand();
void execBuiltInCmd(char* command);
void executeCommand(char* command);
bool isBackgroundJob(char* command); //goes through a linked list to see if the command is paused
void waitPid(int childPid);
bool isCmdEmpty(char*); 
//#endif
