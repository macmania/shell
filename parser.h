struct parseInfo{
  char* command;
}parseInfo; 

struct commandType



void init_info(parseInfo *p);
void parse_command(char* command, struct commandType *comm);
parseInfo *parse (char *cmdline);
void print_info(parseInfo *info);
void free_info(parseInfo *info);
