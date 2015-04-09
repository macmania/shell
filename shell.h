//#ifndef Shell_h
//#define Shell_h
#include "Parser.h"

typedef int bool;
#define true 1
#define false 0


enum 
BUILT_IN_COMMAND { NOT_BLT_CMD=0, JOBS, CD, HISTORY, EXIT, KILL };

enum 
Error_Messages {LESS_NUM_ARGS=0, MORE_NUM_ARGS, NEED_NUMERIC_ARG, FILE_NAME_INCOR};


	//Foreground is ready

/** Signal handlers **/
void handleSigChld(int);
void handleSigCont(int);
void handleSigStp(int); //Can't be handled, ignored or blocked
void handleSigStop(int); //Can be handled, etc. 
void handleSigTTin(int); //terminal access
void handleSigTTou(int);
void handleSigInt(int);
/** End signal handlers **/

/* Helper methods **/
void printPrompt();
char* readCmdLine();
bool isBltInCmd();
void execBltInCmd(struct parseInfo*);
void launchProcess(struct parseInfo*);
bool isBgJob(struct parseInfo*); //goes through a linked list to see if the command is paused
void waitPid(int);
bool isCmdEmpty(char*); 
void printErrMsg(enum Error_Messages, char*);
//#endif



