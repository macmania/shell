#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "JobControl.h"
//Implementation of methods in job control
//Temporary job control
volatile job *firstJob, *lastJob; 
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
		firstJob = malloc(sizeof(job));
		lastJob = malloc(sizeof(job));
		firstJob->next = newJob; 
		printf("add_job: %d\n",newJob->pgid);
		newJob->previous = (job*)firstJob;
		newJob->next = (job*)lastJob; //this is the last
	}
	else{
		job *temp = firstJob->next; 
		temp->previous = newJob; 
		newJob->next = temp; 
		firstJob->next=newJob;
		printf("add_job: %d\n",newJob->pgid);
	}
	size++; 
}

//have to look for the actual job
//To-do, finish this part
int delete_job(pid_t pgid) {
	job* j = find_job(pgid);

	if(j == NULL) //pgid does not exist
		return 0; 


	//case 1: head -> obj -> tail
	if(j->previous == firstJob){
		firstJob->next = j->next;
		j->next->previous = (job*)firstJob;
	}
	//case 2: head-> obj1-> obj2 -> tail
	else{
		job* temp = j->previous;
		temp->next = j->next;
		j->next->previous = temp;
	}

	free_job(j); 
	size--; 
	return 1;
}

job* find_job(pid_t pgid){
	job *head;

	for(head = (job*) *(&firstJob->next); head; head = head->next) {
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

//to-be implemented

/**
* retrieves job from the list based on job scheduling algorithm
*/
process* get_job(void){
	//To-do
	return NULL;
}

/*
 * terminates the job from the running pipe
 * -still need to hammer the details
 *
 * */
int terminate_job(pid_t id){
	//To-do
	return 0;
}
