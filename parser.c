#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include "parser.h"

#define BUFF 128

Command* parse_file(int fd, int* i){
    Command* c_list = malloc(sizeof(struct command*)*100);
    *i = 0;

    char file_buff[BUFF];
    char comment[BUFF];

    while(read_line(fd,file_buff,BUFF) > 0){

        if (file_buff[0] == '$'){
                Command c = new_command();
                set_execution(c,file_buff);
                set_comment(c,comment);
                c_list[*i] = c;
                *i = *i + 1;
        }

        else{
                strcpy(comment,file_buff);
        }
    }

    return c_list;
}

ssize_t read_line(int fd, char *buf, size_t nbyte){
    int i=0;

    while(i<nbyte-1 && read(fd, buf+i,1)>0 && buf[i] != '\n'){
        i++;
    }

    buf[i] = '\0';

    return i;
}
