/** 
	Naive way of doing job control process
	List that stores the process information. 
	This is the basis for job control
**/
#include <termios.h>

#define MAX_VAR_NUM 10
#define PIPE_MAX_NUM 10
#define FILE_MAX_NUM 40

#define BACKGROUND 0
#define FOREGROUND 1 //running
#define SUSPENDED 2 //blocked
#define RUNNING 3
#define WAIT_INPUT 4 
#define COMPLETED 5 //terminated


#define DIRECT_IN '<'
#define DIRECT_OUT '>'
#define PIPE '|'
#define BACKGROUND_INPUT '&' 
#define NORMAL_CMD 'N'

enum
error_msg_parse {
  FILE_NAME_NOT_FOUND=0,
  UNKNOWN_CMD,
  NO_FILE_ENTERED
};

#define setJobCompleted(j) setJobStatus(j, COMPLETED)
#define setJobSuspended(j) setJobStatus(j, SUSPENDED)
#define setJobContinued(j) setJobStatus(j, FOREGROUND)

#define isJobStopped(j) (isJobStatus(j, SUSPENDED))
#define isJobCompleted(j) (isJobStatus(j, BACKGROUND))
/** Single process **/
typedef struct process {
	char* command; 
	char* ArgVarList[MAX_VAR_NUM];
	int argVarNum;
	struct process *next; 
	pid_t pid; 
	int status;
	
	//struct parseInfo* cmdInfo; //this needs to be replaced
}process; 

/* these contains each of the commands that need to be processed" **/
typedef struct job {
	struct job *next; 
	struct job *previous; 
	process *first_process; 
	pid_t pgid; 
	int jobID; //number of active jobs
	char notified; 
	struct termios tmodes; 
	int stdin, stdout, stderr; //why do i need this??
	int indexJob;
	char* commandStr; 
	int status; 
	char commandType;
	int numPipes;
	char inFile[FILE_MAX_NUM];
	char outFile[FILE_MAX_NUM];
	int isInFile, isOutFile;
}job; 


//void initProcess(process**);
void addProcess(process**, process*);//, struct parseInfo*);
void addProcessEnd(process**, process*); 
void addJob(job**, job*);

job* findJob(job**, pid_t);

int  isJobStatus(job*, int);
void setJobStatus(job*, int);

int  deleteJob(job**, job*);
void deleteAllJobs(void); 


void freeProcess(process**);
void freeJob(job*);

void printCommand(job*);
int  getSize(void);
