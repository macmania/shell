#define MAX_VAR_NUM 10
#define PIPE_MAX_NUM 10
#define FILE_MAX_NUM 40

typedef struct{
  char* command;
  char* varType[MAX_VAR]; //pointer to indicate the type of variations of the command
  int type;
}parseInfo;

typedef struct {
  int inFile; //bool val to check if a file is passed to be read
  int outFile;  //bool val to check if a file is passed to be saved in a file
  int isBackground; //checks to see if the process is going to be background
  int commandType; //checks to see what type of command
  int pipeNum; //number of pipes, <still need to finalize>
  char pipeCommArray[PIPE_MAX_NUM];
  char inFile[FILE_MAX_NUM];
  char outFile[FILE_MAX_NUM];
}commandType;



void init_info(parseInfo *p);
void parse_command(char* command, struct commandType *comm);
parseInfo* parse (char *cmdline);
void print_info(parseInfo *info);
void free_info(parseInfo *info);
