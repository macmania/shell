/** 
	Naive way of doing job control process
	List that stores the process information. 
	This is the basis for job control
**/
#include <termios.h>
#include "Parser.h"

#define COMPLETED 0
/** Single process **/
typedef struct process {
	struct process *next; 
	pid_t pid;
	char stopped; 
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
}job; 

void initProcess(void);
job* findJob(pid_t); 
int isJobStopped(job*);
int isJobCompleted(job*);
void addJob(job*); 
int deleteJob(pid_t pgid); //returns 0 or 1 if job was successfully 
						//deleted in the job pipeline and freed 
					//successfully
void deleteAllJobs(void); 
int getSize(void);
void freeProcess(process*);
void freeJob(job*);
void setCobCompleted(job*);

//To-do
process* getJob(void);
int terminateJob(pid_t);
void printCommand(struct commandType*);
