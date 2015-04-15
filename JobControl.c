#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "JobControl.h"

//Implementation of methods in job control
//Temporary job control
 //list to hold the stopped jobs
volatile int size;


void initProcess(process* p) {
	p = malloc(sizeof(p)); 
}

//adds the first job 
//need to call parse_job first before adding the job. 
//makes this confusing if not handled appropriately
void addJob(job* newJob){
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

int deleteJob(pid_t pgid) {
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

job* findJob(pid_t pgid){
	job *head;

	for(head = (job*) ((*firstJob)->next); head; head = head->next) {
		if(head->pgid == pgid) {
			return head;
		}
	}
	return NULL; 
}

//deletes all of the processes in the pipe-line
void freeProcess(process *head){
	struct process *tmp;
	while(head != NULL){
		tmp = head; 
		head = head->next; 
		free(tmp); 
	}
}

//do i need to free termios variable?
void freeJob(job* j){
	if(j->first_process != NULL){
		free_process(j->first_process);
	}

	j->next = NULL; 
	j->previous = NULL;
	freeInfo(j->cmdInfo);
	free(j->command);
	free(j); 
}



/** utility functions that will be used to operate on job objects **/
int isJobStopped(job *head){
	process *temp; 

	for(temp = (process*)head; temp; temp = temp->next)
		if(temp->status == SUSPENDED && !temp->stopped)
			return 0; 
	return 1; 
}

int isJobCompleted(job *head){
	process *p; 
	for(p = (process*)head->first_process; p; p = p->next)
		if(p->status == BACKGROUND)
			return 0; 
	return 1; 
}

void setJobStatus(job *j, int status){
	if(j == NULL){
		printf("ERROR... SET_JOB_STATUS"); //temporary debug flag
		return;
	}
	process *p;
	for(p = (process*)j->first_process; p; p = p->next){
		p->status = status;
	}
}

void setJobCompleted(job* j){
	set_job_status(j, COMPLETED);
}

void setJobSuspended(job* j){
	set_job_status(j, SUSPENDED);
}

void setJobContinued(job* j){
	set_job_status(j, FOREGROUND);
}

int getSize(void){
	return size;
}

void printCommand(commandType *command){
	int i;

	if(command->isInFile)
		printf("%s <", command->inFile);
	
	//need to reclarify this partd
	if(command->numPipes == 0){// && command->commandType == NORMAL_CMD){
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
