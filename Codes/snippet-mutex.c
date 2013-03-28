/** Code snippet : Mutex
  * This is a sample code explaining how mutex works.
  * The goal is to print the 5 first multiple of a given value in one
  * thread and to square this given value in another thread.
  */

#include <stdio.h>      // Input/Output
#include <pthread.h>    // Threads management

// Creating mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Global given value
int value = 2;

void* print_first_multiples(void* nothing) {
    // Iterator
    int i;

    // MUTEX - LOCK
    pthread_mutex_lock( &mutex );

    printf("First multiples of %d are:\n", value);

    for(i = 1 ; i <= 5 ; i++) {
        printf("\t %d x %d = %d\n", value, i, value*i);
    }

    // MUTEX - UNLOCK
    pthread_mutex_unlock( &mutex );
    return NULL;
}

void* square_value(void* nothing) {
    // MUTEX - LOCK
    pthread_mutex_lock( &mutex );
    
    printf("Square of %d is ..", value);
    value = value*value;
    printf("... %d.\n", value);
    
    // MUTEX - UNLOCK
    pthread_mutex_unlock( &mutex );

    return NULL;
}

// Main function
int main(int argc, char* argv[]) {
    // Threads declaration
    pthread_t thread1;
    pthread_t thread2;

    // Creating threads
    pthread_create(&thread1, NULL, print_first_multiples, NULL);
    pthread_create(&thread2, NULL, square_value, NULL);

    // Wait the end of all threads	
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
