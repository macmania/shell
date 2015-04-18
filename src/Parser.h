/**
* Need to dynamically change the size of the arg var list in the future. 
*/

#define MAX_VAR_NUM 10
#define PIPE_MAX_NUM 10
#define FILE_MAX_NUM 40

#define BACKGROUND 0
#define FOREGROUND 1
#define SUSPENDED 2
#define RUNNING 3
#define WAIT_INPUT 4
#define COMPLETED 5


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


struct parseInfo{
  char* command;
  char* ArgVarList[MAX_VAR_NUM]; //pointer to indicate the type of variations of the command
  							//don't get why to put this attribute here, to-do
  int argVarNum;
};

//full commando parsed command
typedef struct {
  int isInFile; //bool val to check if a file is passed to be read
  int isOutFile;  //bool val to check if a file is passed to be saved in a file
  int status; //checks to see if the process is going to be background
  char commandType; //checks to see what type of command
  int numPipes; //number of pipes, <still need to finalize>
  struct parseInfo CmdArray[PIPE_MAX_NUM];
  char inFile[FILE_MAX_NUM];
  char outFile[FILE_MAX_NUM];
}commandType;


void initInfo(struct parseInfo*);
void parseCommand(char*, commandType*);
struct parseInfo* parse (char*);
void printInfo(struct parseInfo*);
void freeInfo(struct parseInfo*);
void freeCmdType(commandType*);
void printError(enum error_msg_parse);
int isFile(char* fileName); 
char* trimWhiteSpaces(char*);
int isProperFile(char*);
