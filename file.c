#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char buf[1024];

    int fd;
    fd=open("saida.nb", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    
    int fi;
    fi = open("exemplo.nb", O_RDONLY);
    
    int fe;
    fe = open("erros.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
      
    if (fe == -1) {
        perror("Erro ao abrir o ficheiro erros.txt");
        _exit(-1);
    }

    dup2(fe, 2);
    close(fe);
 
    perror("Teste!\n");
     
    if (fd == -1) {
        perror("Erro ao abrir o ficheiro output.txt");
        _exit(-1);
    }
    
    if (fi == -1) {
        perror("Erro ao abrir o ficheiro passwd");
        _exit(-1);
    }
 
    dup2(fd, 1);
    close(fd);
 
    dup2(fi, 0);
    close(fi);

    read(0, buf, 1024);
    
    int i = 0;
    int ii = 0;
    int j = 0;
    char command[256];
    
    for (i = 0; buf[i] != '\0'; i++) {
        if (buf[i] == '$' && buf[i+1] != '|') {
            printf(">>>\n");
            for (j = i + 2; buf[j] != '\n'; j++) {
                command[ii] = buf[j];
                ii++;
            }
            
            FILE *fp;
            char path[1035];

            /* Open the command for reading. */
            fp = popen(command, "r");
            if (fp == NULL) {
                printf("Failed to run command\n" );
                exit(1);
            }

            /* Read the output a line at a time - output it. */
            while (fgets(path, sizeof(path)-1, fp) != NULL) {
                printf("%s", path);
            }
            pclose(fp);

            printf("<<<\n");
            i = i + ii + 3;
        }
        memset(command, 0, 256);
        ii = 0;
        printf("%c", buf[i]);
    }
    return 0;
}
