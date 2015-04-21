#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "JobControl.h"

//Implementation of methods in job control
//Temporary job control
 //list to hold the stopped jobs

volatile int size; //testing purpose


void initProcess(process** p) {
	*p = malloc(sizeof(process)); 
}

//adds the first job 
//need to call parse_job first before adding the job. 
//makes this confusing if not handled appropriately
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

void addProcess(process** head, process* p, struct parseInfo *cmd){
	if(p == NULL) {
		perror("Cannot add an empty process"); fflush(0); 
		return;
	}
	
	p->cmdInfo = cmd;
	
	if(head == NULL){
		p->next = NULL;
		*head = (process*) p;
	}
	else{
		p->next = *head;
		*head = p; 
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
	struct process *tmp;
	while(*head != NULL){
		tmp = *head; 
		*head = (*head)->next; 
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
	freeCmdType(j->command);
	free(j->commandStr);
	free(j->command);
	
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

void printCommand(commandType *command){
	int i;

	if(command->isInFile)
		printf("%s <", command->inFile);
	
	//need to reclarify this partd
	if(command->numPipes == 0){// && command->commandType == NORMAL_CMD){
		printInfo(&(command->CmdArray[0]));
	}
	else if(command->numPipes > 0){
		for(i = 0; i < command->numPipes ;i++){
			printInfo(&(command->CmdArray[i]));
		}
	}
	if(command->isOutFile){
		printf("%s <", command->outFile);
	}
	if(command->commandType == BACKGROUND_INPUT){
		printf(" &");
	}
}
