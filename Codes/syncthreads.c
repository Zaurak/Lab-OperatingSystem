/** Lab Subject :
  * 2 threads to increment a shared counter
  * 1 thread to wait that a given amount is reached
  */
  
#include <stdio.h>      // Input/Output
#include <unistd.h>     // Useful for usleep() function
#include <pthread.h>    // Threads management

pthread_cond_t  cond_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t id_mutex = PTHREAD_MUTEX_INITIALIZER;

int counter = 0;    // Global counter

#define LIMIT 10    // Global LIMIT constant

// Simple function to get a comprehensive unique ID for threads
int getId() {
    static int id = 0;
    pthread_mutex_lock(&id_mutex);      // MUTEX - LOCK
    id++;
    pthread_mutex_unlock(&id_mutex);    // MUTEX - UNLOCK
    return id;
}

// 
void* increment(void* nothing) {
    int	exit = 0;   // Loop condition
    int shortId = getId();   // Compute a short ID

    while(!exit) {   	// Infinite loop
    
        pthread_mutex_lock(&mutex);         // MUTEX - LOCK

        if (counter >= LIMIT) {     // CONDITION VARIABLE CHECK
            printf("[INCREMENT] %d : SIGNAL SENT\n", shortId);
            pthread_cond_signal(&cond_var);	// Send signal
            exit = 1;       // Break the loop and terminate thread
        } else {
            // Print a trace of the thread activity
            printf("[INCREMENT] %d : Counter from '%d' to '%d'\n", 
            shortId, counter, counter + 1);

            counter = counter + 1;      // Increment
        }

        pthread_mutex_unlock(&mutex);     // MUTEX - UNLOCK

        // Wait 1ms to avoid that only one thread appear in the output
        usleep(1000); 
    }

    // "Natural" termination of the thread
    printf("\tTermination of [INCREMENT] %d\n", shortId);
    return NULL;
}

void* isReached(void* nothing) {

    int shortId = getId();

    pthread_mutex_lock(&mutex);         // MUTEX - LOCK

    // Wait for signal from increment threads
    if (counter < LIMIT) { // Avoid missing signals skipping cond_wait
        printf("[ISREACHED] %d : WAITING SIGNAL\n", shortId);
        pthread_cond_wait(&cond_var, &mutex);
    }
    // Trace of the received signal
    printf("[ISREACHED] %d : SIGNAL RECEIVED\n", shortId);
    printf("[ISREACHED] %d : Counter = %d\n", shortId, counter);
	
    pthread_mutex_unlock(&mutex);     // MUTEX - UNLOCK

    // "Natural" termination of the thread
    printf("\tTermination of [ISREACHED] %d\n", shortId);
    return NULL;
}

// Main function
int main(int argc, char* argv[]) {
    printf("Synchronized Threads Lab\n");

    // Iterator
    int i;

    // Array of threads
    pthread_t thread[3];

    // Array of thread return values
    int iret[3];

    // Creating threads
    iret[0] = pthread_create(&thread[0], NULL, increment, NULL);
    iret[1] = pthread_create(&thread[1], NULL, increment, NULL);
    iret[2] = pthread_create(&thread[2], NULL, isReached, NULL);

    // Wait until threads are all complete
    for (i = 0 ; i < 3 ; i++)
        pthread_join(thread[i], NULL);

    // Print the result (return value) of all threads
    for (i = 0 ; i < 3 ; i++)
        printf("Thread %d returns: %d\n", i, iret[i]);

    return 0;
}
