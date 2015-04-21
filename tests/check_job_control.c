#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/JobControl.h"
//#include "../src/Parser.h"

job** firstJob; 
 
START_TEST(test_initProcess)
{
	process *p; 
	
	initProcess(&p);
	
	if(p == NULL){
		ck_abort_msg("initProcess() failed\n");
	}
	free(p);
}
END_TEST

START_TEST(test_addJob)
{
	job *newJob, *newJob2;
	newJob = malloc(sizeof(job)); 
	newJob2 = malloc(sizeof(job));
	
	newJob->pgid = 123; 
	newJob->commandStr = malloc(sizeof(char)*30);
	strcpy(newJob->commandStr, "ls -l");
	
	newJob2->pgid = 124; 
	newJob2->commandStr = malloc(sizeof(char)*30); 
	strcpy(newJob2->commandStr, "cd jojofabe");
	
	if(*firstJob == NULL){
		ck_abort_msg("first job is null");
	}
	ck_assert_ptr_ne	(firstJob, NULL);
	
	ck_assert_int_eq(getSize(), 0);
	addJob(firstJob, NULL);
	ck_assert_int_eq(getSize(), 0);
	
	addJob(firstJob, newJob); 
	ck_assert_ptr_eq(*firstJob, newJob);
	ck_assert_int_eq((*firstJob)->pgid, 123);
	ck_assert_int_eq(getSize(), 1);
	
	addJob(firstJob, newJob2); 
	ck_assert_ptr_eq(*firstJob, newJob2); 
	ck_assert_int_eq(getSize(), 2); 
	
	ck_assert_ptr_ne(findJob(firstJob, 123), NULL);
	ck_assert_ptr_eq(findJob(firstJob, 145), NULL); 
	
	free(newJob);
	free(newJob2); 
}
END_TEST

START_TEST(test_addProcess)
{
	process* p; 
	initProcess(&p);
	
	if(p == NULL){
		ck_abort_msg("process p is null");
	}
	ck_assert_ptr_ne(p, NULL);
	
	//initializing process
	p->pid = 145; 
	p->status = SUSPENDED;
	
	ck_assert_ptr_eq((*firstJob)->first_process, NULL);
	addProcess(&(*firstJob)->first_process, p, NULL);
	ck_assert_ptr_eq(((*firstJob)->first_process), p);
	ck_assert_int_eq((*firstJob)->first_process->pid, 145);
	ck_assert_int_eq((*firstJob)->first_process->status, SUSPENDED);
}
END_TEST

START_TEST(test_deleteJob)
{
	job* j = malloc(sizeof(job));
	job* j2 = malloc(sizeof(job)); 
	
	ck_assert_int_eq(deleteJob(firstJob, NULL), 0); 
	
	addJob(firstJob, j); 
	ck_assert_ptr_eq(*firstJob, j); 
	
	addJob(firstJob, j2); 
	ck_assert_ptr_eq(*firstJob, j2);
	
	deleteJob(firstJob, j2); 
	ck_assert_ptr_eq(*firstJob, j);
	
	deleteJob(firstJob, j); 
	ck_assert_int_eq(getSize(), 0); 
}
END_TEST

START_TEST(test_freeProcess)
{
	process* p; 
	process* p1; 
	if(p == NULL || p1 == NULL) 
		ck_abort_msg("p or p1 is null"); 
	 
	initProcess(&p);
	initProcess(&p1);
	ck_assert_ptr_ne(p, NULL); 
	ck_assert_ptr_ne(p1, NULL); 
	ck_assert_ptr_eq((*firstJob)->first_process, NULL); 
	addProcess(&((*firstJob)->first_process), p1, NULL);
	ck_assert_ptr_eq((*firstJob)->first_process, p1);
	addProcess(&(*firstJob)->first_process, p, NULL); 
	ck_assert_ptr_eq((*firstJob)->first_process, p);
	 
	freeProcess(&((*firstJob)->first_process)); 
	ck_assert_ptr_ne((*firstJob)->first_process, p); 
}
END_TEST


START_TEST(test_isJobStatus)
{
	process *t1, *t2, *t3, *t4, *t5; 
	t1 = malloc(sizeof(process)); 
	t1->status = RUNNING;
	
	t2 = malloc(sizeof(process)); 
	t2->status = RUNNING;
	
	t3 = malloc(sizeof(process));
	t3->status = RUNNING;
	
	t4 = malloc(sizeof(process)); 
	t4->status = SUSPENDED;
	
	t5 = malloc(sizeof(process)); 
	t5->status = RUNNING;
	
	addProcess(&(*firstJob)->first_process, t3, NULL);
	addProcess(&(*firstJob)->first_process, t2, NULL);
	addProcess(&(*firstJob)->first_process, t1, NULL);
	
	ck_assert_ptr_eq((*firstJob)->first_process, t1); 
	ck_assert_int_eq(isJobStopped(*firstJob), 1); 
	
	addProcess(&(*firstJob)->first_process, t4, NULL); 
	ck_assert_ptr_eq((*firstJob)->first_process, t4);
	
	addProcess(&(*firstJob)->first_process, t5, NULL);
	ck_assert_ptr_eq((*firstJob)->first_process, t5);
	
	ck_assert_int_eq(isJobStopped(*firstJob), 0);
	
	setJobCompleted(*firstJob); 
	ck_assert_int_eq(isJobCompleted(*firstJob), 1);
	 
	setJobSuspended(*firstJob);
	ck_assert_int_eq(isJobStopped(*firstJob), 0);
	
	freeProcess(&(*firstJob)->first_process); 
}
END_TEST



Suite* job_control_suite(void){
	Suite* s; 
	TCase *tc_core; 
	
	s = suite_create("Job Control"); 
	tc_core = tcase_create("Core"); 
	suite_add_tcase(s, tc_core);	
	tcase_add_test(tc_core, test_initProcess);
	tcase_add_test(tc_core, test_addJob);
	tcase_add_test(tc_core, test_addProcess);
	tcase_add_test(tc_core, test_deleteJob);
	tcase_add_test(tc_core, test_freeProcess);
	tcase_add_test(tc_core, test_isJobStatus);
	
	return s;
}

int main(void){

	int numberFailed; 
	Suite* j; 
	SRunner *sr; 
	firstJob = malloc(sizeof(job*));
	*firstJob = malloc(sizeof(job));
	//initProcess(&((*firstJob)->first_process));
	j = job_control_suite(); 
	sr = srunner_create(j); 
	
	srunner_run_all(sr, CK_VERBOSE); 
	numberFailed = srunner_ntests_failed(sr); 
	srunner_free(sr); 
	
	if((*firstJob) == NULL){
		free(*firstJob);
		free(firstJob);
	} 
	else{
		freeJob(*firstJob);
	}
	
	return (numberFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE; 
}