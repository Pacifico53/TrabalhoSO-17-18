#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

#include "command.h"
#include "parser.h"

int write_to_file(Command* c_list, int fd, int size); void increase(char** data);


int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Error in processment call. Missing arguments\n");
    }

    int file_descriptor = open(argv[1],O_RDONLY);
    int size;

    Command* c_list = parse_file(file_descriptor,&size);
    close(file_descriptor);

    for(int i = 0; i < size; i++){
        int link[2];
        int link2[2];
        if(pipe(link) < 0) perror("Pipe error");
        if(pipe(link2) < 0) perror("Pipe 2 error");

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

                int n_read = 0, out_size = 0; 
                int max = 2048;
                char* out_buff = malloc(max);
                char* aux_buff = malloc(max);
                
                strcat(out_buff,">>>\n");
                while((n_read = read(link[0],aux_buff,1024)) > 0){
                    if(max < n_read + out_size){
                        max *= 2;
                        increase(&out_buff);
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
        else if(len == 2){
                p_id = fork();
                if(p_id < 0) perror("Fork error");

                if (p_id == 0) {
                    close(link[0]);
                    dup2(link[1], 1);
                    close(link[1]);
        
                    close(link2[1]);
                    dup2(link2[0], 0);
                    close(link2[0]);
                    
                    execvp(trim[0], trim);
                    perror("Execution error");
                    _exit(1);
                }
                else {
                    close(link[1]);

                    char* outputBefore = get_output(c_list[i-1]);
                    int outLen = strlen(outputBefore);
                    outputBefore[outLen - 4] = '\0';
                    outputBefore = outputBefore + 4;
                    write(link2[1], outputBefore, strlen(outputBefore));
                    close(link2[1]);

                    wait(&out);
                    
                    int n_read = 0, out_size = 0; 
                    int max = 2048;
                    char* out_buff = malloc(max);
                    char* aux_buff = malloc(max);
                    
                    strcat(out_buff, ">>>\n");
                    while((n_read = read(link[0], aux_buff, 1024)) > 0){
                        if(max < n_read + out_size){
                            max *= 2;
                            increase(&out_buff);
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
        else if(len > 2){
            int typeLen = strlen(type);
            type[typeLen - 1] = '\0';
            type = type + 1;
            int index = atoi(type);
            p_id = fork();
                if(p_id < 0) perror("Fork error");

                if (p_id == 0) {
                    close(link[0]);
                    dup2(link[1], 1);
                    close(link[1]);
        
                    close(link2[1]);
                    dup2(link2[0], 0);
                    close(link2[0]);
                    
                    execvp(trim[0], trim);
                    perror("Execution error");
                    _exit(1);
                }
                else {
                    close(link[1]);

                    char* outputBefore = get_output(c_list[i-index]);
                    int outLen = strlen(outputBefore);
                    outputBefore[outLen - 4] = '\0';
                    outputBefore = outputBefore + 4;
                    write(link2[1], outputBefore, strlen(outputBefore));
                    close(link2[1]);

                    wait(&out);
                   
                    int n_read = 0, out_size = 0; 
                    int max = 2048;
                    char* out_buff = malloc(max);
                    char* aux_buff = malloc(max);

                    strcat(out_buff, ">>>\n");
                    while((n_read = read(link[0], aux_buff, 1024)) > 0){
                        if(max < n_read + out_size){
                            max *= 2;
                            increase(&out_buff);
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

void increase(char** data){
    *data = realloc(*data, 1024 * sizeof(char));
}
