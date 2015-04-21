/** 
	Naive way of doing job control process
	List that stores the process information. 
	This is the basis for job control
**/
#include <termios.h>
#include "Parser.h"

#define setJobCompleted(j) setJobStatus(j, COMPLETED)
#define setJobSuspended(j) setJobStatus(j, SUSPENDED)
#define setJobContinued(j) setJobStatus(j, FOREGROUND)

#define isJobStopped(j) (isJobStatus(j, SUSPENDED))
#define isJobCompleted(j) (isJobStatus(j, BACKGROUND))
/** Single process **/
typedef struct process {
	struct process *next; 
	pid_t pid; 
	int status;
	struct parseInfo* cmdInfo;
}process; 

/* these contains each of the commands that need to be processed" **/
typedef struct job {
	struct job *next; 
	struct job *previous; 
	process *first_process; 
	pid_t pgid; 
	int jobID; //number of active jobs
	char notified; 
	struct termios tmodes; 
	int stdin, stdout, stderr; //why do i need this??
	int indexJob;
	commandType *command;
	char* commandStr; //might want to change this in the future to 
					//to explicitly only contain an array of char. 
}job; 


void initProcess(process**);
job* findJob(job**, pid_t);
int  isJobStatus(job*, int);
void setJobStatus(job*, int);
int  deleteJob(job**, job*);
void addJob(job**, job*);
void deleteAllJobs(void); 
int  getSize(void);
void freeProcess(process**);
void freeJob(job*);
void addProcess(process**, process*, struct parseInfo*);
void printCommand(commandType*);
