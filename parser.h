/**
	Need to dynamically change the size of the arg var list somehow
*/

#define MAX_VAR_NUM 10
#define PIPE_MAX_NUM 10
#define FILE_MAX_NUM 40

enum
error_msg_parse {
  FILE_NAME_NOT_FOUND=0,
  UNKNOWN_CMD,
  NO_FILE_ENTERED
};

enum
command_type {
  ENTER_FILE=0, 
  DEPART_FILE,
  BACKGROUND_CMD,
  PIPE
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
  int isBackground; //checks to see if the process is going to be background
  int commandType; //checks to see what type of command
  int numPipes; //number of pipes, <still need to finalize>
  struct parseInfo CmdArray[PIPE_MAX_NUM];
  char inFile[FILE_MAX_NUM];
  char outFile[FILE_MAX_NUM];
}commandType;



void init_info(struct parseInfo*);
void parse_command(char*, commandType*);
struct parseInfo* parse (char*);
void print_info(struct parseInfo*);
void free_info(struct parseInfo*);
char* get_cmds(void);
void print_error(enum error_msg_parse);
int is_file(char* fileName); 
char* trimWhiteSpaces(char*);
int is_proper_file(char*);
void set_pipes(char[], commandType*);