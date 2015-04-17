/*
 * Got from Fikret Ercal, (Ph.D, The Ohio State University) 
 * Signal Example with wait() and waitpid()
 *
 * Author: Brian Sea
 * Date: 9/17/03
 */

#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int i, parent, child, status;

static void signal_handler(int signo)//, siginfo_t* t, void* context)
{
	int pid;
	int status;
    /* signo contains the signal number that was received */
    switch( signo )
    {
        /* Signal is a SIGINT */
        case SIGINT:
        		sleep(1);
            printf( "Process %d: received SIGINT (Ctrl-C)\n", getpid() );
            fflush(stdout);
            printf( "Process %d: passing SIGINT to %d...\n", getpid(),child );
            kill( getpid(), SIGKILL);
            break;
        
        /*  It's a SIGQUIT */
        case SIGQUIT:
            printf( "Process %d: received SIGQUIT (Ctrl-\\)\n", getpid() );
            fflush(stdout);
            printf( "Process %d: passing SIGQUIT to %d...\n", getpid(),child );
            kill( child, SIGQUIT );
            break;
        case SIGCONT:
        		printf("Process %d: received SIGCONT\n", getpid());
        		fflush(stdout);
        		break;
        case SIGCHLD:

        		printf("Process %d: received SIGCHLD\n", getpid());
        		while((pid = wait(&status))){};
        		break;
        default:
                break;
    }

    return;
}



int main( int argc, char *argv[], char *env[] )
{
    if( signal( SIGINT, SIG_IGN) == SIG_ERR  )
    {
        printf("Parent: Unable to create handler for SIGINT\n");
    }

    if( signal( SIGQUIT, SIG_IGN) == SIG_ERR  )
    {
        printf("Parent: Unable to create handler for SIGQUIT\n");
    }
    signal(SIGCONT, signal_handler); signal(SIGCHLD, signal_handler);//, NULL);
    parent = getpid();
    printf( "Parent pid = %d\n", parent );
    	while(1){
		if( (child = fork()) == 0 )
		{
			/* Ignore the signals that are supposed to be received by the parent*/
			signal( SIGQUIT, SIG_IGN );

			/* Restore the default handler on SIGINT in the child */
			signal( SIGINT, signal_handler );

			signal(SIGCONT, signal_handler);
			signal(SIGCHLD, SIG_IGN);
			printf( "Child pid = %d\n", getpid() );
			for( ;; )
			{
				sleep( 1 );
			}
		}
		else
		{
			int pid = wait(&status);
			printf("%d ID", pid);

			if( WIFEXITED( status ) )
			{

				printf( "Child return - %d\n", WEXITSTATUS( status ) );
			}
			else
			{
				/* Well it didn't exited properly.  Was it a signal? */
				if( WIFSIGNALED( status ) )
				{
					/*
					 * Yes. A signal killed the child process.  Now we can extract
					 * the signal information from status
					 */

					printf( "Child died on signal - %d\n", WTERMSIG( status ));
				}
			}

			/*
			 * There are two other macros most UNIXes use.  They are:
			 *  WIFSTOPPED() and WSTOPSIG().  See the man pages on the dells for
			 *  more information.
			 *
			 *  To wait on a particular pid - see waitpid()
			 */
		}
    	}
    return 0;
}
