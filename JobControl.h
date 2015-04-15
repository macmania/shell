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

job* find_job(pid_t); 
int is_job_stopped(job*);
int is_job_completed(job*);
void add_job(job*); 
int delete_job(pid_t pgid); //returns 0 or 1 if job was successfully 
						//deleted in the job pipeline and freed 
					//successfully 
int get_size(void);
void free_process(process*);
void free_job(job*);
void set_job_completed(job*);

//To-do
process* get_job(void);
int terminate_job(pid_t);
void print_command(struct commandType*);
