/** Code snippet : Join
  * This is a sample code explaining how join works.
  * The goal is to compute/print the average value of a really long
  * list of numbers. 
  * We will use 2 threads to compute the sum and 1 (the main) to divide
  * it by the number of records.
  *                     Inspired by the Monte-Carlo Algorithm.
  */

#include <stdio.h>      // Input/Output
#include <stdlib.h>     // Useful for rand() function
#include <pthread.h>    // Threads management

#define NB_REC 1000                                 // Records number

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Creating mutex

int recordList[NB_REC];                             // Records array

int result = 0;                                     // Result variable

// Define the range of the array to be computed by the thread
typedef struct range {
    int start;
    int end;
}t_range;

void* compute_sum(void* range) {

    // Initialisation
    t_range r = *((t_range*) range);    // Cast conversion
    int i;                              // Iterator
    int partialSum = 0;                 // Partial sum
    
    // Computation
    for (i = r.start ; i < r.end ; i++) {
        partialSum += recordList[i];
    }
    
    // Synchronisation of the partial sums
    pthread_mutex_lock( &mutex );       // MUTEX - LOCK
    result += partialSum;
    pthread_mutex_unlock( &mutex );     // MUTEX - UNLOCK

    return NULL;
}

// Main function
int main(int argc, char* argv[]) {
    // Iterator
    int i;
    
    // Threads declaration
    pthread_t thread1;
    pthread_t thread2;
    
    // struct range declaration/initialisation
    t_range r1 = {0, NB_REC/2};
    t_range r2 = {NB_REC/2, NB_REC};
    
    // Initialising fake values for the array
    for (i = 0 ; i < NB_REC ; i++) {
        recordList[i] = rand() % 101 + 1;   // value range: [1, 100]
    }

    // Creating threads
    pthread_create(&thread1, NULL, compute_sum, (void*) &r1);
    pthread_create(&thread2, NULL, compute_sum, (void*) &r2);

    // Wait the end of thread 1 & 2
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    // We are now sure that the sum is complete
    result = result / NB_REC;
    
    // Print the result
    printf("Result = %d\n", result);
    
    return 0;
}
