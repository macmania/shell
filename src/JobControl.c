#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "JobControl.h"

//Implementation of methods in job control
//Temporary job control
 //list to hold the stopped jobs

volatile int size; //testing purpose

void addJob(job** firstJob, job* newJob){
	if(newJob == NULL){
		return; 
	}

	if(firstJob == NULL){
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


void addProcessEnd(process** head, process *newProcess){
	if(newProcess == NULL) return;
	
	if(*head == NULL){
		newProcess->next = NULL;
		*head = (process*) newProcess;
		return;
	}
	else{
		process* current = *head; 
		for(current = *head; current->next; current = current->next);
		current->next = newProcess;
	}
}


job* findJob(job** firstJob, pid_t pgid){
	job *head;

	for(head = (job*) (*firstJob); head; head = head->next) {
		if(head->pgid == pgid) {
			return head;
		}
	}
	return NULL; 
}


int deleteJob(job** firstJob, job* j) {
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

	freeJob(j); 
	size--; 
	return 1;
}

//deletes all of the processes in the pipe-line
void freeProcess(process **head){
	if(*head == NULL) 
		return;
	struct process *tmp;
	int i; 
	while((*head)->next != NULL){
		tmp = *head; 
		*head = (*head)->next; 
		free(tmp->command);

		for(i = 0; i < tmp->argVarNum; i++)
			free(tmp->ArgVarList[i]);
		
		free(tmp); 
	}
	head = NULL; 
}

//do i need to free termios variable?
void freeJob(job* j){
	if(j->first_process != NULL){
		freeProcess(&(j)->first_process);
	}

	j->next = NULL; 
	j->previous = NULL;
	free(j->commandStr);
	free(j); 
}



/** utility functions that will be used to operate on job objects **/
int isJobStatus(job *head, int status){
	process *temp; 

	for(temp = (process*)head->first_process; temp; temp = temp->next)
		if(temp->status == status)
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

int getSize(void){
	return size;
}

void printCommand(job* j){
	int i;

	printf("Command: %s", j->commandStr);
}