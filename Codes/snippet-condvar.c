/** Code snippet : Condition Variables
  * This is a sample code explaining how condition variables works.
  * The goal is to show the Collatz Conjecture : take a random number,
  * if it's even, divide by 2, if it's odd, multiply by 3 and add 1.
  * The conjecture say that it should reach 1 at some point.
  * 1 thread will handle even numbers
  * 1 thread will handle odd numbers
  */

#include <stdio.h>      // Input/Output
#include <stdlib.h>     // Useful for atoi() function
#include <pthread.h>    // Threads management
#include <unistd.h>     // Useful for usleep() function

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Creating mutex
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER; // Creating cond_var

// Random number
int value;

void* collatz_even(void* nothing) {
    
    for(;;) {               // Infinite loop
        // MUTEX - LOCK
        pthread_mutex_lock( &mutex );
        printf("[EVEN THREAD] LOCK\n");

        if (value % 2 == 0) {   // If it's even
            printf("[EVEN THREAD] value: %2d => %2d\n", value, value/2);
            value = value / 2;
            printf("%d\n", value);
        } else {                // If it's odd
            // Signal sent to stop waiting of (one) other thread
            pthread_cond_signal(&cond_var);	
            printf("[EVEN THREAD] value: %2d => SIGNAL SENT\n", value);
        }
        
        // MUTEX - UNLOCK
        printf("[EVEN THREAD] UNLOCK\n");
        pthread_mutex_unlock( &mutex );
        
        // Make a short pause to let time to the other thread 
        // to receive the signal. This avoid useless loops in this
        // even thread
        usleep(1000);    // Wait 1 ms
        
        // End condition
        if (value == 1) {
            // Send signal so that the other thread don't wait forever
            pthread_cond_signal(&cond_var); 
            return NULL;
        }
    }
}

void* collatz_odd(void* nothing) {

    for(;;) {               // Infinite loop
        // MUTEX - LOCK
        pthread_mutex_lock( &mutex );
        printf("[ODD THREAD] LOCK\n");
        

        // Wait for signal
        printf("[ODD THREAD] WAITING SIGNAL\n");
        pthread_cond_wait(&cond_var, &mutex);	
        printf("[ODD THREAD] SIGNAL RECEIVED\n");
        
        // Avoid going in infinite loop (1, 4, 2, 1, 4, 2, 1....)
        if (value != 1) {
            // Do the operation
            printf("[ODD THREAD] value: %2d  => %2d\n", 
                value, value*3 + 1);
            value = value*3 + 1;
            printf("%d\n", value);
        }
        // MUTEX - UNLOCK
        printf("[ODD THREAD] UNLOCK\n");
        pthread_mutex_unlock( &mutex );
        
        // End condition
        if (value == 1) return NULL;
    }
}

// Main function
int main(int argc, char* argv[]) {

    // Getting the starting value from the prompt
    if (argc != 2) {
        fprintf(stderr, "Usage: %s starting_val\n", argv[0]);
        return -1;
    }
    value = atoi(argv[1]);
    
    printf("Starting Collatz Conjecture with value:\n");
    printf("%d\n", value);
    
    // Threads declaration
    pthread_t thread1;
    pthread_t thread2;

    // Creating threads
    pthread_create(&thread1, NULL, collatz_even, NULL);
    pthread_create(&thread2, NULL, collatz_odd, NULL);

    // Wait the end of all threads	
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
