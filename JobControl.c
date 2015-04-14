#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "JobControl.h"

//Implementation of methods in job control
//Temporary job control
volatile job **firstJob;
volatile int size;


void init_process(process* p) {
	p = malloc(sizeof(p)); 
}

//adds the first job 
//need to call parse_job first before adding the job. 
//makes this confusing if not handled appropriately
void add_job(job* newJob){
	if(newJob == NULL){
		return; 
	}

	if(firstJob == NULL){
		firstJob = (volatile struct job**) malloc(sizeof(struct job));
		newJob->next = NULL; 
		newJob->previous = NULL;
		*firstJob = newJob; 
	}
	else {
		(*firstJob)->previous = newJob;
		newJob->next = (job*) *firstJob;
		*firstJob = newJob;
	}
	size++; 
}

int delete_job(pid_t pgid) {
	job* j = find_job(pgid);

	if(j == NULL) 
		return 0; 

	if(j == *firstJob){
		*firstJob = j->next; 
	}
	else if (j->next != NULL){
		j->next->previous = j->previous;
	}
	else if(j->previous != NULL){
		j->previous->next = j->next;
	}

	free_job(j); 
	size--; 
	return 1;
}

job* find_job(pid_t pgid){
	job *head;

	for(head = (job*) ((*firstJob)->next); head; head = head->next) {
		if(head->pgid == pgid) {
			return head;
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
	if(j->firstProcess != NULL){
		free_process(j->firstProcess);
	}

	j->next = NULL; 
	j->previous = NULL;
	free(j); 
}



/** utility functions that will be used to operate on job objects **/
int is_job_stopped(job *head){
	process *temp; 

	for(temp = (process*)head; temp; temp = temp->next)
		if(!temp->completed && !temp->stopped)
			return 0; 
	return 1; 
}

int is_job_completed(job *head){
	process *p; 
	for(p = (process*)head->firstProcess; p; p = p->next)
		if(!p->completed)
			return 0; 
	return 1; 
}

int get_size(void){
	return size;
}
