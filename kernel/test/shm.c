#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#include "producer.h"
#include "consumer.h"

int main()
{
    pid_t pid;
    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    
    if(pid > 0){
        producer();
    }
    else{
        consumer();
        printf("whatthe");
    }
    return 0;
}
