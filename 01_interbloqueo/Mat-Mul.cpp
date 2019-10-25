// g++ -pthread program_name.cpp

// CPP Program to multiply two matrix using pthreads 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
  
// maximum size of matrix 
#define MAX 1000
  
// maximum number of threads 
#define MAX_THREAD 10
  
int matA[MAX][MAX]; 
int matB[MAX][MAX]; 
int matC[MAX][MAX]; 

double total_time;
clock_t clk_start, clk_end;
int step = -1;
  
void* multi(void* arg) 
{ 
    int core = ++step;
  
    // Each thread computes 1/nth of matrix multiplication 
    for (int i = core * MAX / MAX_THREAD; i < (core + 1) * MAX / MAX_THREAD; i++)  
        for (int j = 0; j < MAX; j++)  
            for (int k = 0; k < MAX; k++)  
                matC[i][j] += matA[i][k] * matB[k][j]; 
    
    pthread_exit(NULL);
    return NULL;
}
  
// Driver Code 
int main()
{
    // Generating random values in matA and matB 
    for (int i = 0; i < MAX; i++) { 
        for (int j = 0; j < MAX; j++) { 
            matA[i][j] = rand() % 10; 
            matB[i][j] = rand() % 10; 
        } 
    }
  
    // declaring four threads 
    pthread_t threads[MAX_THREAD]; 

    clk_start = clock();  
    // Creating four threads, each evaluating its own part 
    for (int idx = 0; idx < MAX_THREAD; idx++) {
        pthread_create(&(threads[idx]), NULL, multi, NULL); 
    } 
    
    // joining and waiting for all threads to complete 
    for (int i = 0; i < MAX_THREAD; i++)  
        pthread_join(threads[i], NULL);
    
    clk_end= clock();

    total_time = ((double) (clk_end - clk_start))/(1000.0*1000.0);
    printf("Total time: %lf\n", total_time);
    return 0; 
}
