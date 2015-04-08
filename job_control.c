//Implementation of methods in job control
//Temporary job control
#include "parser.h"

volatile job *firstJob = NULL; 
volatile int size = 0; 
//this can always be changed in the future
//putting all of the fields in cmd to job j
//(potential problem): cmd needs to be checked before calling init_job
void parse_job(commandType* cmd, job *j){
	/*Imperative that they initialize j in shell.c or else can't get the 
		pointer**/

	//to-be continued.

}

void init_process(process* p) {
	p = malloc(sizeof(p)); 
}

//parses each of the commands respectively 
process* parse_cmd(){
	//to-be continued.
}


//adds the first job 
//need to call parse_job first before adding the job. 
//makes this confusing if not handled appropriately
void add_job(commandType* cmd, job* j){
	if(j == NULL)
		return; 

	j = parse_job(cmd, j); //sets up the contents specified in cmd to parse_job

	if(firstJob == NULL){
		firstJob = malloc(sizeof(job));
		firstJob->next = j; 
		j->previous = firstJob;
		j->next = NULL; //this is the last
	}
	else{
		job *temp = firstJob->next; 
		temp->next = newJob; 
		newJob->previous = temp; 
		firstJob->next=newJob;
	}
	size++; 
}

//have to look for the actual job
void delete_job(pid_t pgid) {
	job* j = find_job(pgid);

	job* temp = j->previous; 
	temp->next = j->next; 
	j->next->previous = temp; 

	free_job(j); 
}

job* find_job(pid_t pgid){
	job *head, *returnJob;

	for(head = firstJob; head; head = head->next) {
		if(head->pgid == pgid) {
			returnJob = head; 
			return returnJob;
		}
	}
}

//deletes all of the processes in the pipe-line
void free_process(process *head){
	struct process *tmp;
	while(head != NULL){
		tmp = head; 
		head = head->next; 
		free(tmp); 
	}
}

//do i need to free termios variable?
void free_job(job* head){
	free_process(j->firstProcess);
	j->next = NULL; 
	j->previous = NULL;
	free(j); 
}



/** utility functions that will be used to operate on job objects **/
int is_job_stopped(job *head){
	process *temp; 
	for(temp = head; temp; temp = head->next)
		if(!temp->completed && !temp->stopped)
			return 0; 
	return 1; 
}

int is_job_completed(job *head){
	process *p; 
	for(p = head->firstProcess; p; p = head->next)
		if(!p->completed)
			return 0; 
	return 1; 
}