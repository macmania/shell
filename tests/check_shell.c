/**
 * To-do 
 * 
 * */
#include <check.h>
#include "../src/Shell.h"
#include <stdlib.h>
#include <stdio.h>

job firstJob**; 

START_TEST(test_readyJob)
{
	
	ck_assert_int_eq(0, 0);
	printf("Hello");
}
END_TEST

START_TEST(test_launchJob)
{

}
END_TEST

START_TEST(test_setJobBackground )
{

}
END_TEST

START_TEST(test_setJobForeground )
{

}
END_TEST

START_TEST(test_waitForJob )
{

}
END_TEST

START_TEST(test_markJobAsRunning )
{

}
END_TEST

START_TEST(test_continueJob )
{

}
END_TEST

START_TEST(test_isBltInCmd)
{

}
END_TEST


START_TEST(test_isCmdEmpty)
{

}
END_TEST

START_TEST(test_killAllJobs)
{

}
END_TEST

START_TEST(test_getJob)
{

}
END_TEST


Suite* shell_suite(void){
	Suite *s; 
	TCase *tc_core; 
	
	s = suite_create("Shell");
	
	tc_core = tcase_create("Core");
	
	tcase_add_test(tc_core, test_readyJob);
	suite_add_tcase(s, tc_core);
	
	return s;
}

int main(void) 
{
	int numberFailed; 
	Suite* s; 
	SRunner *sr; 
	firstJob = malloc(sizeof(job));
	
	s = shell_suite(); 
	sr = srunner_create(s); 
	
	srunner_run_all(sr, CK_NORMAL);
	numberFailed = srunner_ntests_failed(sr);
	srunner_free(sr);
	

	//killAllJobs();

	free(job);
	return (numberFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
	
}

