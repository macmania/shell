//Implementation of methods in job control
//Temporary job control
#include "parser.h"

//this can always be changed in the future
//putting all of the fields in cmd to job j
//(potential problem): cmd needs to be checked before calling init_job
job* parse_job(job *j, commandType* cmd){
	/*Imperative that they initialize j in shell.c or else can't get the 
		pointer**/



}

void init_process(process* p) {
	p = malloc(sizeof(p)); 
}

//parses each of the commands respectively 
process* parse_cmd(){

}

void add_job(job *head, job* newJob){
	if(j == NULL)
		return; 
	job *temp = head->next; 
	temp->next = newJob; 
	newJob->previous = temp; 
	head->next=newJob;


}


job* find_job(pid_t pgid, job* head){

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
	free(j); 
}