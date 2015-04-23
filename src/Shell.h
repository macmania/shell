//#ifndef Shell_h
//#define Shell_h
#include "Parser.h"
#include <signal.h> 

#define TRUE 1
#define FALSE 0

#define BY_PROCESS_ID 0
#define BY_JOB_STATUS 1
#define BY_JOB_ID 3 //the id number of the job list

enum 
BUILT_IN_COMMAND { NOT_BLT_CMD=0, JOBS, CD, HISTORY, EXIT, KILL };


enum 
Error_Messages {LESS_NUM_ARGS=0, MORE_NUM_ARGS, NEED_NUMERIC_ARG, FILE_NAME_INCOR};

job **firstJob;
static pid_t shell_pgid;
static int shell_terminal, shell_interactive;
static int sizeStoppedJobs;
struct termios shell_tmodes;


void init(void); //initialize signal handlers

/** Signal handlers **/
void sigChldHandler(int, siginfo_t*, void*);
void sigHandlers(int, siginfo_t*, void*);

/** End signal handlers **/

/* Helper methods **/
void printPrompt();
char* readCmdLine();
int isBltInCmd(process*);
void execBltInCmd(process*);
void launchProcess(job*);
job* getJob(pid_t, int);
void launchJob(job*);
void readyJob(char*, job**);
int isBackgroundJob(job*); //goes through a linked list to see if the command is paused

void setJobForeground(job*, int);
void setJobBackground(job*, int);
void waitForJob(job*);
void continueJob(job*, int);
void markJobAsRunning(job*);
int isCmdEmpty(char*);
void printErrMsg(enum Error_Messages, char*);
void killAllJobs(void);
//#endif



