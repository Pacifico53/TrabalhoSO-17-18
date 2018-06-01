#ifndef _COMMAND_H_
#define _COMMAND_H_

typedef struct command *Command;

Command new_command();

void set_comment(Command c, char* desc);
void set_execution(Command c, char* com);
void set_output(Command c, char* out);

char* get_comment(Command c);
char* get_execution(Command c);
char* get_output(Command c);

char** trim_execution(Command c);

#endif
