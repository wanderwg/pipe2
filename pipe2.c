#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    //ls | grep make
    //
    int pipefd[2];
    int ret=pipe(pipefd);
    if(ret<0){
        perror("pipe error");
        return -1;
    }
    int pid1=fork();
    if(pid1==0){
        //ls
        close(pipefd[0]);
        dup2(pipefd[1],1);
        execlp("ls","ls",NULL);
        exit(0);
    }
    int pid2=fork();
    if(pid2==0){
        //grep make
        close(pipefd[1]);
        dup2(pipefd[0],0);
        execlp("grep","grep","make",NULL);
        exit(0);
    }
    close(pipefd[1]);
    close(pipefd[0]);
    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);
    return 0;
}
