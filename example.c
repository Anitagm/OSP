#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


int sum = 0; // Global int to store the sum of the two arrays
sem_t mutex; // Synchronization Bit

void *add(void *arg){
    int *ptr = (int *) arg;
    while(*ptr != -1){
        sem_wait(&mutex);
        sum += *ptr;
        sem_post(&mutex);
        ptr++;
    }
    return NULL;
}

int main(int argc, char *args[]){
    clock_t t; 
    t = clock();
    int A[4] = {1,2,3, -1}; // -1 marks the end of array
    int B[4] = {4,2,6, -1};

    pthread_t t_a, t_b;
    sem_init(&mutex, 0, 1);
    pthread_create(&t_a , NULL, add, A);
    pthread_create(&t_b, NULL, add, B);

    pthread_join(t_a, NULL);
    pthread_join(t_b, NULL);
    printf("Total: %d\n", sum);
     
    
    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
 
    printf("fun() took %f seconds to execute \n", time_taken); 

    return 0;
}