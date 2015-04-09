/** Simple tests to check correctness of JobControl
 	These are simple tests that aim to 
 		- add a job
 		- delete a job
 		- find a job from its pgid
 		- find if a job has completed
 		- find if a job has stopped
*/
#include <stdio.h>
#include <stdlib.h>
#include "JobControl.h"
int main(void){

	job *j = malloc(sizeof(job));
	job *j1 = malloc(sizeof(job)); 

	j->pgid = 12; 

	add_job(j); add_job(j1);
	printf("deleted job j: %s\n", delete_job(12) == 1 ? "true" : "false" ); 
	printf("deleted job j: %s\n", delete_job(-1) == 1 ? "true" : "false" ); 

	printf("size: %d", get_size());
	free(j);

	exit(0);
}
