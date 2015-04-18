/**
 * To-do 
 * 
 * */
#include <check.h>
#include "../src/Shell.h"



START_TEST(test_readyJob)
{
	ck_assert_int_eq(0, 0);
	printf("Hello");
}
END_TEST

int main(void) 
{
	firstJob = malloc(sizeof(job));
	
	killAllJobs();
	free(job);
	return 0;
	
}

