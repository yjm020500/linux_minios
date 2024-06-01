#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int fork_ex()
{
    char write_msg[BUFFER_SIZE] = "Greetings \n";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    pid_t pid;
    if(pipe(fd)==-1){
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    /* fork a child process */
    pid = fork(); //pid(p)>0: pid of child process; pid(c): 0
    if (pid<0){  /* error occurred. Child starts here */
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if(pid==0){ /*child process*/
    /*close the unused end of the pipe*/
    close(fd[WRITE_END]);
    /*read from the pipe*/
    read(fd[READ_END], read_msg, BUFFER_SIZE);
    printf("read %s", read_msg);
    /*close the read end of the pipe*/
    close(fd[READ_END]);
    exit(0);
    }
    else{ /*parent process*/
    close(fd[READ_END]); //close the unused end of the pipe
    write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
    /*close the write end of the pipe*/
    close(fd[WRITE_END]);
    wait(NULL);
    }
    return 0;
}
