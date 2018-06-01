#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

#include "command.h"
#include "parser.h"

int write_to_file(Command* c_list, int fd, int size);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Error in processment call. Missing arguments");
    }

    int file_descriptor = open(argv[1],O_RDONLY);
    int size;

    Command* c_list = parse_file(file_descriptor,&size);
    close(file_descriptor);

    for(int i = 0; i < size; i++){

        int link[2];
        if(pipe(link) < 0) perror("Pipe error");

        char* type = strtok(get_execution(c_list[i])," ");
        char** trim = trim_execution(c_list[i]);
        int len = strlen(type);

        pid_t p_id;
        int out;

        if(len == 1){
            p_id = fork();
            if(p_id < 0) perror("Fork error");

            if (p_id == 0) {
                close(link[0]);
                dup2(link[1], 1);
                close(link[1]);

                execvp(trim[0],trim);
                perror("Execution error");
                _exit(1);
            }
            else {
                close(link[1]);
                wait(&out);

                int n_read, out_size;
                char* out_buff = malloc(1024);
                char* aux_buff = malloc(1024);

                strcat(out_buff,">>>\n");
                while((n_read = read(link[0],aux_buff,1024)) > 0){
                    if(sizeof(out_buff) < n_read + out_size){
                        char* aux = realloc(out_buff, sizeof(out_buff)*2);
                        if(!aux){
                            perror("Realloc error");
                            _exit(1);
                        }
                        out_buff = aux;
                    }
                    strcat(out_buff,aux_buff);
                    out_size += n_read;
                }
                strcat(out_buff,"<<<\n");

                set_output(c_list[i],out_buff);

                free(out_buff);
                free(aux_buff);
            }
        }
    }

    file_descriptor = open(argv[1],O_WRONLY|O_TRUNC);
    write_to_file(c_list,file_descriptor,size);
    close(file_descriptor);

    return 0;
}

int write_to_file(Command* c_list, int fd, int size){

    for (int i = 0; i < size; ++i){
        char *comment = get_comment(c_list[i]);
        strcat(comment,"\n");
        write(fd, comment, strlen(comment));

        char *exec = get_execution(c_list[i]);
        strcat(exec,"\n");
        write(fd, exec, strlen(exec));

        char *output = get_output(c_list[i]);
        write(fd, output, strlen(output));
    }

    return 0;
}