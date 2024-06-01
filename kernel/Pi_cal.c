#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h> //added for sync

#define thread_num 10 //thread number
#define N 10000000


struct point{
    double x;
    double y;
};


int num[thread_num]; //save the number of points in a circle
struct point points[N];
sem_t sem; //added for sync


void set_point();
void *cal_pi(void *);
bool cal_distance(double, double);
void *check_num(void *);


void Pi_cal() //main
{
    sem_init(&sem, 0, 1); //added for sync
    
    set_point();
    pthread_t tid[thread_num]; //the thread identifier
    
    for (long int i=1;i<=thread_num;i++){  //create thread
        pthread_create(&tid[i],NULL,cal_pi,(void *)i); 
    }
    
    for(long int i =0;i<thread_num;i++){ //wait thread
        pthread_join(tid[i], NULL);
    }
}


void set_point() //choose the points
{   srand(time(NULL));
    for (long int i=0;i<N;i++){
        points[i].x = (double)rand()/RAND_MAX; //-1~1
        points[i].y = (double)rand()/RAND_MAX; //-1~1
    }
}


bool cal_distance(double x, double y){ //calculate the distance
    double diff = (x*x) + (y*y);
    if(diff <= 1){
        return 1;
    }
    else{
        return 0;
    }
}


void *cal_pi(void *param) //calcuate the pi (thread)
{
    long int thread_id = (long int)param;
    pthread_t tid2;
    pthread_create(&tid2,NULL,check_num,(void *)thread_id);
    pthread_join(tid2, NULL);
    
    sem_wait(&sem); //wait semaphore for Pi, added for sync
    double pi = 4.0*(num[thread_id])/((N/thread_num)*thread_id); //pi calculation
    printf("in circle is %d and total is %ld \n", num[thread_id], ((N/thread_num)*thread_id));// print
    printf("Pi is %f \n", pi);
    sleep(1);
    sem_post(&sem); //release semaphore, added for sync
    
    pthread_exit(0);
}


void *check_num(void *param) //count the number of in_points (thread2)
{   long int thread_id = (long int)param;
    num[thread_id]=0;
    for(long int i=0;i<((N/thread_num)*thread_id);i++){ //count number
        if(cal_distance(points[i].x, points[i].y)){
            num[thread_id]=num[thread_id]+1;
        }
    }
    pthread_exit(0);
}



