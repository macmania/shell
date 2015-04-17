#define MAX_NUM 10

#include <stdio.h>
#include <stdlib.h>



typedef struct commands { 
	char* args[MAX_NUM];
	int pid; 
}commands;

struct commands* cmd[MAX_NUM]; 
char* args_ls[3], args_cd[2], args_man[2], args_echo[3];
int pointerIndex; 
int main(void){
	cmd[0] = malloc(sizeof(struct commands));
	cmd[1] = malloc(sizeof(struct commands));
	cmd[2] = malloc(sizeof(struct commands));
	cmd[3] = malloc(sizeof(struct commands));

	// args_ls[0]="ls"; 		args_ls[1]="-l"; 	   args_ls[2]=NULL;
	// /** char* args_cd[2]; **/	args_cd[0]="cd"; 		args_ls[1]=NULL;
	// * char* args_man[2]; *	args_man[0]="man"; 		args_man[1]=NULL;
	// /** char* args_echo[3];**/  args_echo[0]="echo";	args_echo[1]="$PATH";  args_echo[2]=NULL;


	cmd[0]->args[0] = "ls"; cmd[0]->args[1] = NULL; 
	cmd[1]->args[0] = "whois"; cmd[1]->args[1] = "jojofabe"; cmd[1]->args[3] = NULL; 
	cmd[2]->args[0] = "man"; cmd[2]->args[1] = NULL; 
	cmd[3]->args[0] = "echo"; cmd[3]->args[1] = "$PATH"; cmd[3]->args[2] = NULL; 

	




	int i;
	for(i = 0; i < 4; i++)
		free(cmd[i]);

}