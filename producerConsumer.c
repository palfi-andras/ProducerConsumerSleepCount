/*
Andras Palfi
Operating System 4300 
10/14/2016
Programing Assignment 2 
Part B2
 
 A simple implementation of the
 Producer-Consumer problem (read more: https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem)
 written in C. This implementation utilizes
 an adjustable Buffer (queue) size, mutex variables,
 and is set to loop for 1000 productions (concurrently using 2
 producer threads) and 1000 consumptions (with 1 consumer thread), 
 although these values are adjustable. The purpose of this is to
 return the number of times the producer and
 consumer went to sleep due to the wait conditions.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define Queue_Limit 50

// A max size for producer/consumers
int MAX_SIZE = 1000;      
int NumItemsProd;
int NumItemsCon;

//The start index for the queue
int Index_Value = 0;
char *Queue;
//The mutex variable that will be used
pthread_mutex_t mutex_variable = PTHREAD_MUTEX_INITIALIZER;

//I defined two conditions for the Queue not empty/full conditions  

pthread_cond_t Queue_Full = PTHREAD_COND_INITIALIZER;
pthread_cond_t Queue_Empty = PTHREAD_COND_INITIALIZER;

//The Producer and Consumer count variables used in the main method to 
//limit to 1000 items produced/consumed
int Consumer_Sleep_Count = 0;
int Producer_Sleep_Count = 0;


//Consumer method
void *Consumer()
{
    //Repeat while true
       while(1)
       {
          //Lock the mutex
           pthread_mutex_lock(&mutex_variable);
           //If the index is -1 then the queue is empty so we need to wait 
           //I also incremeented the consumer sleep count variable here because 
           //the consumer is effectivley sleeping at this moment 
           if(Index_Value == -1)
           {            
               pthread_cond_wait(&Queue_Empty, &mutex_variable);
               Consumer_Sleep_Count += 1;

           }                
          

           //Unlock the Mutex variable because the process has exited critical
           //region     
           pthread_mutex_unlock(&mutex_variable);        
           //Signal that the queue has been emptied 
           pthread_cond_signal(&Queue_Full);    
                      
       }    
      
}


//Producer method
void *Producer()
{    


    while(1)
        {   
         
          //Lock the mutex variable
            pthread_mutex_lock(&mutex_variable);

            //For the producer we need to check if the index is equal to the 
            //amount of storage we have in our queue. If the queue is full
            //We cannot add more to it so we need to wait. Also we increment
            //the producer sleep count variable
            if(Index_Value == Queue_Limit)
            {                        
                pthread_cond_wait(&Queue_Full, &mutex_variable);
                Producer_Sleep_Count += 1; 
            }   
            //Increase the index vaue of the queue
            Queue[Index_Value++] = '@';
        
            //unlock the mutex variable
            pthread_mutex_unlock(&mutex_variable);
            //Signal that the queue has been filled
            pthread_cond_signal(&Queue_Empty);  
               
        }    
             
}

int main()
{    
    //increment variables used to limit 1000 producers/consumer later
    int i,j;
    //declare thread ids
    pthread_t producer_thread_idA, producer_thread_idB, consumer_thread_id;
    //Create the queue
    Queue = (char *) malloc(sizeof(char) * Queue_Limit);            
    
//Run two simulatenous producer threads and one consumer thread until we reach 1000 runs
    for(i=0; i<MAX_SIZE; i++){
      pthread_create(&producer_thread_idA, NULL, Producer, NULL);
      pthread_create(&producer_thread_idB,NULL,Producer,NULL);

    }
    for(j=0; j<MAX_SIZE; j++){
      pthread_create(&consumer_thread_id, NULL, Consumer, NULL);
      }

//Join the producer threads to suspend their execution
    if (i==MAX_SIZE-1){
          pthread_join(producer_thread_idA, NULL);
          pthread_join(producer_thread_idB, NULL);
      }

//Suspend the consumer thread execution
    if (j==MAX_SIZE-1){
        pthread_join(consumer_thread_id, NULL);
      }

    //Print the counts of each Producer and Consumer
    printf("Consumer Sleep Count:  %d\n", Consumer_Sleep_Count);
    printf("Producer Sleep Count:   %d\n", Producer_Sleep_Count);
    //End the program execution
    return 0;
}
