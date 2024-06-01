#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

#define N 10 //10 processes
#define TQ 3 //The size of the time quantum

struct Process{
    int id; //process id
    int burst_time; //process burst_time
    int remain_time; //remain_time
    bool is_completed; //completion status
    pid_t pid;
};

void initialize();
void round_robin_scheduling();
bool is_all_completed();
void processing(int, int);

struct Process processes[N];

int RR_scheduler() {  //main
    printf("TQ is %d for round robin scheduling \n", TQ);
   
    srand(time(NULL));
    initialize();
   
    printf("\n");
   
    round_robin_scheduling();
   
    return 0;
}

void initialize(){ //initialization
    for(int i=0;i<N;i++){
        processes[i].id = i;
        processes[i].burst_time = rand()%10+1; //random burst time
        processes[i].remain_time = processes[i].burst_time;
        processes[i].is_completed = false;
        processes[i].pid = 0;
        printf("process %d, burst_time is %d \n", i, processes[i].burst_time);
    }
}

void round_robin_scheduling() { //round_robing_scheduling
    while(true){
        for(int i=0;i<N;i++){
            processes[i].pid = fork(); // make processes
            if(processes[i].pid < 0){ //error
                fprintf(stderr, "Fork failed");
                exit(1);
            }
            else if(processes[i].pid==0){ //child process
                while(1){
                    pause();
                    
                } //wait signal
            }
            else{ //parent process
                if(processes[i].remain_time > 0){ //not completed process
                    if(processes[i].remain_time > TQ){ //more than TQ
                        processing(i,TQ); //processing child process for TQ
                        processes[i].remain_time = processes[i].remain_time - TQ;
                        printf("process %d processed, remain_time is %d, turnaround_time is %d \n", i, processes[i].remain_time, TQ);
                    }
                    else{ //equal and less than TQ
                        processing(i,processes[i].remain_time); // processing child process for remain_time
                        int remain = processes[i].remain_time;
                        processes[i].remain_time = 0;
                        processes[i].is_completed = true;
                        printf("process %d processed, remain_time is 0 that is, processing completed, turnaround_time is %d \n", i, remain);
                    }
                }
            }
        }
        if(is_all_completed()){ //test all completed
            printf("all processes completed!!\n");
            break;
        }
    }
    for(int i=0;i<N;i++){
        kill(processes[i].pid, SIGKILL); //exit the child processes
    }
}

bool is_all_completed(){  //test all completed
    int count=0;
    for(int i = 0;i<N;i++){
        if(processes[i].is_completed == true){
            count++;
        }
    }
    if(count == 10){
        return 1;
    }
    else {
        return 0;
    }
}

void processing(int i, int time){ //child process processing
    kill(processes[i].pid, SIGCONT);
    sleep(time);
    kill(processes[i].pid, SIGSTOP);
}
