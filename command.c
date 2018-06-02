#include <stdlib.h>
#include <memory.h>
#include <zconf.h>
#include "command.h"

struct command{
    char* comment;
    char* execution;
    char* output;
};

Command new_command(){
    Command com = malloc(sizeof(struct command));
    com->comment = malloc(1024);
    com->execution = malloc(64);
    com->output = malloc(1024);

    return com;
}

void set_comment(Command c, char* desc){
    strcpy(c->comment,desc);
}

void set_execution(Command c, char* exec){
    strcpy(c->execution,exec);
}

void set_output(Command c, char* out){
    if(strlen(out) > 1024) realloc(c->output, strlen(out));
    strcpy(c->output,out);

}

char* get_comment(Command c){
    char* comm = strdup(c->comment);
    return comm;
}

char* get_execution(Command c){
    char* exec = strdup(c->execution);
    return exec;
}

char* get_output(Command c){
    char* out = strdup(c->output);
    return out;
}

char** trim_execution(Command c){
    char** trim = malloc(sizeof(char* )*16);
    char* aux = get_execution(c);

    char *type = strtok(aux, " ");

    for(int i = 0; type; ++i){
        type = strtok(NULL," ");
        trim[i] = type;
    }

    return trim;
}
