#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//创建一个管道
//pipe_fd[0]:read
//pipe_fd[1]:write
int pipe_fd[2];

pid_t child_pid;

void childTask();
void parentTask();

int main(int argc, const char *argv[]){
    printf("Pipe loading....\n");

    if(pipe(pipe_fd)==-1){
        printf("Cannot create the pipe!\n");
        exit(EXIT_FAILURE);
    }

    printf("---------FORKING---------\n");

    //fork子进程：pid为0fork失败，0为子进程，其他为父进程
    if((child_pid = fork()) == -1){
        perror("Process cannot be fork");
        exit(EXIT_FAILURE);
    }

    if(child_pid == 0){
        childTask();
    }
    else{
        parentTask();
    }
    return 0;

}

void parentTask(){
    //父进程不需要写
    close(pipe_fd[1]);

    char buffer[128];
    if(read(pipe_fd[0],buffer,128)==-1){
        printf("Cannot read pipe\n");
        exit(EXIT_FAILURE);
    }

    close(pipe_fd[0]);

    printf("PARENT - Received: %s\n", buffer);
    exit(EXIT_SUCCESS);
}

void childTask(){
    close(pipe_fd[0]);

    char *buffer="This is child!";
    printf("CHILD - Sending : %s\n", buffer);

    write(pipe_fd[1],buffer,strlen(buffer)-1);
    close(pipe_fd[1]);
    exit(EXIT_SUCCESS);
}

