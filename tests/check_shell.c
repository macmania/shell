/**
 * To-do 
 * 
 * */
#include <check.h>
#include "../src/Shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
 

START_TEST(test_readyJob)
{	
	char cmdStr[] = "ls -l"; 
	
	struct parseInfo* firstCmd = malloc(sizeof(parseInfo));
	
	
	commandType* cmd = malloc(sizeof(commandType));
	
	
	job* j = readyJob(cmdStr, firstCmd, cmd, malloc(sizeof(job)));
	
	free(cmd); 
	free(parseInfo);
	
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
	tcase_add_test(tc_core, test_launchJob);
	tcase_add_test(tc_core, test_setJobBackground);
	tcase_add_test(tc_core, test_setJobForeground);
	tcase_add_test(tc_core, test_waitForJob);
	tcase_add_test(tc_core, test_markJobAsRunning);
	tcase_add_test(tc_core, test_continueJob );
	tcase_add_test(tc_core, test_isBltInCmd);
	tcase_add_test(tc_core, test_isCmdEmpty);
	tcase_add_test(tc_core, test_killAllJobs);
	tcase_add_test(tc_core, test_getJob);

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

	free(*firstJob);
	return (numberFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
	
}

