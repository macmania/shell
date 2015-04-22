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
	
	struct parseInfo* firstCmd = malloc(sizeof(struct parseInfo));
	firstCmd->command = "ls"; 
	strcpy(firstCmd->ArgVarList, "-l");
	firstCmd->argVarNum = 1;
	
	commandType* cmd = malloc(sizeof(commandType));
	cmd->isInFile = FALSE;
	cmd->numPipes = 0; 
	job* j = malloc(sizeof(job));
	
	ck_assert_ptr_ne(j, NULL);
	readyJob(cmdStr, firstCmd, cmd, &j); 
	
	ck_assert_ptr_ne(j->first_process, NULL); //pointer should be allocated
	ck_assert_ptr_eq(j->first_process->cmdInfo, firstCmd);
	
	//Simulating piping commands 
	cmd->numPipes = 3; //ls -l | grep -x "Apr" | ls -l
	
	cmd->CmdArray[0].command = "ls"; 
	strcpy(cmd->CmdArray[0].ArgVarList,"-l");
	cmd->CmdArray[0].argVarNum = 1;

	cmd->CmdArray[1].command = "grep";
	strcpy(cmd->CmdArray[1].ArgVarList, "-x");
	cmd->CmdArray[1].argVarNum = 1;
	
	cmd->CmdArray[2].command = "ls";
	strcpy(cmd->CmdArray[2].ArgVarList, "-l");
	cmd->CmdArray[2].argVarNum = 1;
	
	readyJob(cmdStr, firstCmd, cmd, &j);
	
	ck_assert_int_eq(j->command->numPipes, 3);
	
	int i;
	
	
	//for(i = 0; i )
	
	//freeCmdType(cmd);
	free(cmd);
	//freeInfo(firstCmd);
	free(firstCmd);
	free(j);
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
	srunner_set_fork_status(sr, CK_NOFORK);
	srunner_run_all(sr, CK_NORMAL);
	numberFailed = srunner_ntests_failed(sr);
	srunner_free(sr);
	

	//killAllJobs();

	//free(*firstJob);
	return (numberFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
	
}

