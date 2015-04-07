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
typedef struct job {
	struct job *next; 
	struct commandType command;
	process *firstProcess; 
	pid_t pgid; 
	char notified; 
	struct termios tmodes; 
	int stdin, stdout, stderr; 
}job; 

job* find_job(pid_t pgid); 
job_is_stopped(job *j);
job_is_completed(job *j);