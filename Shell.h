//#ifndef Shell_h
//#define Shell_h
#include "JobControl.h"

#define TRUE 1
#define FALSE 0



#define BY_PROCESS_ID 0
#define BY_JOB_STATUS 1
#define BY_JOB_ID 3 //the id number of the job list

enum 
BUILT_IN_COMMAND { NOT_BLT_CMD=0, JOBS, CD, HISTORY, EXIT, KILL };


enum 
Error_Messages {LESS_NUM_ARGS=0, MORE_NUM_ARGS, NEED_NUMERIC_ARG, FILE_NAME_INCOR};


void init(void); //initialize signal handlers

/** Signal handlers **/
void sigChldHandler(int, siginfo_t*, void*);
void sigHandlers(int, siginfo_t*, void*);

/** End signal handlers **/

/* Helper methods **/
void printPrompt();
char* readCmdLine();
int isBltInCmd();
void execBltInCmd(job*);
void launchProcess(job*);
int isBackgroundJob(job*); //goes through a linked list to see if the command is paused

void put_job_foreground(job*, int);
void put_job_background(job*, int);
void wait_for_job(job*);
void continue_job(job*, int);
void mark_job_as_running(job*);
int isCmdEmpty(char*);
void printErrMsg(enum Error_Messages, char*);
//#endif



