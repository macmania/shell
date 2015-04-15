#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "JobControl.h"

//Implementation of methods in job control
//Temporary job control
volatile job **firstJob; //list to hold the stopped jobs
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
	if(j->first_process != NULL){
		free_process(j->first_process);
	}

	j->next = NULL; 
	j->previous = NULL;
	free(j); 
}



/** utility functions that will be used to operate on job objects **/
int is_job_stopped(job *head){
	process *temp; 

	for(temp = (process*)head; temp; temp = temp->next)
		if(temp->status == SUSPENDED && !temp->stopped)
			return 0; 
	return 1; 
}

int is_job_completed(job *head){
	process *p; 
	for(p = (process*)head->first_process; p; p = p->next)
		if(p->status == BACKGROUND)
			return 0; 
	return 1; 
}

void set_job_status(job *j, int status){
	if(j == NULL){
		printf("ERROR... SET_JOB_STATUS"); //temporary debug flag
		return;
	}
	process *p;
	for(p = (process*)j->first_process; p; p = p->next){
		p->status = status;
	}
}

void set_job_completed(job* j){
	set_job_status(j, COMPLETED);
}

void set_job_suspended(job* j){
	set_job_status(j, SUSPENDED);
}

void set_job_continued(job* j){
	set_job_status(j, FOREGROUND);
}

int get_size(void){
	return size;
}

void print_command(commandType *command){
	int i;

	if(command->isInFile)
		printf("%s <", command->inFile);

	if(command->numPipes == 0 && commandType == NORMAL_CMD){
		print_info(&(command->CmdArray[0]));
	}
	else if(command->numPipes > 0){
		for(i = 0; i < command->numPipes ;i++){
			print_info(&(command->CmdArray[i]));
		}
	}
	if(command->isOutFile){
		printf("%s <", command->outFile);
	}
	if(command->commandType == BACKGROUND_CMD){
		printf(" &");
	}
}
