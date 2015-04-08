/** 
	Naive way of doing job control process
	List that stores the process information. 
	This is the basis for job control
**/
#include "parser.h"
enum
Process_State {
	FOREGROUND=0, 
	BACKGROUND, 
	RUNNING, 
	TERMINATED
};

/** Single process **/
typedef struct process {
	struct process *next; 
	pid_t pid; 
	char completed; 
	char stopped; 
	int status; 
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
	//struct commandType command;
	process *firstProcess; 
	pid_t pgid; 
	char notified; 
	struct termios tmodes; 
	int stdin, stdout, stderr; //why do i need this??
}job; 

job* find_job(pid_t); 
job_is_stopped(job*);
job_is_completed(job*);
void add_job(job*); 
int delete_job(job*); //returns 0 or 1 if job was successfully 
						//deleted in the job pipeline and freed 
					//successfully 
