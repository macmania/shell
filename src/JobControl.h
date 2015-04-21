/** 
	Naive way of doing job control process
	List that stores the process information. 
	This is the basis for job control
**/
#include <termios.h>
#include "Parser.h"

/** Single process **/
typedef struct process {
	struct process *next; 
	pid_t pid; 
	int status;
	struct parseInfo* cmdInfo;
}process; 

/* pipeline of processes */
/* might want to use a hash-table for keeping a list of 
   jobs, easier access
	To-do. 
***/
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
int isJobStopped(job*);
int isJobCompleted(job*);
int deleteJob(job**, job*); //returns 0 or 1 if job was successfully 
						//deleted in the job pipeline and freed 
					//successfully
void addJob(job**, job*);
void deleteAllJobs(void); 
int getSize(void);
void freeProcess(process*);
void freeJob(job*);
void setJobCompleted(job*);
void setJobSuspended(job*);
void setJobContinued(job*);
void addProcess(process**, process*, struct parseInfo*);
//To-do
//process* getJob(void);
int terminateJob(pid_t);
void printCommand(commandType*);
