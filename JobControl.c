#include <stdlib.h>
#include <unistd.h>
#include "JobControl.h"
//Implementation of methods in job control
//Temporary job control
volatile job *firstJob; 
volatile int size;


void init_process(process* p) {
	p = malloc(sizeof(p)); 
}

//adds the first job 
//need to call parse_job first before adding the job. 
//makes this confusing if not handled appropriately
void add_job(job* newJob){
	if(newJob == NULL)
		return; 

	if(firstJob == NULL){
		firstJob = malloc(sizeof(job));
		firstJob->next = newJob; 
		newJob->previous = (job*)firstJob;
		newJob->next = NULL; //this is the last
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
int delete_job(pid_t pgid) {
	job* j = find_job(pgid);

	if(j == NULL) //pgid does not exist
		return 0; 

	job* temp = j->previous; 
	temp->next = j->next; 
	j->next->previous = temp; 

	free_job(j); 
	size--; 
	return 1;
}

job* find_job(pid_t pgid){
	job *head, *returnJob;

	for(head = firstJob; head; head = head->next) {
		if(head->pgid == pgid) {
			printf("HERE"); 
			returnJob = head; 
			return returnJob;
		}
	}
	return NULL; 
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
void free_job(job* j){
	free_process(j->firstProcess);
	j->next = NULL; 
	j->previous = NULL;
	free(j); 
}



/** utility functions that will be used to operate on job objects **/
int is_job_stopped(job *head){
	process *temp; 
	for(temp = (job*)head; temp; temp = head->next)
		if(!temp->completed && !temp->stopped)
			return 0; 
	return 1; 
}

int is_job_completed(job *head){
	process *p; 
	for(p = (job*)head->firstProcess; p; p = head->next)
		if(!p->completed)
			return 0; 
	return 1; 
}

int get_size(void){
	return size;
}