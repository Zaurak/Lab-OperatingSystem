/** Lab Subject :
  * Simulate a race condition using threads
  */

#include <stdio.h>      // Input/Output
#include <pthread.h>    // Threads management

#define N_THREAD 10

int account = 0;

void* deposit(void* nothing) {
    int i;
    for (i = 0 ; i < 10 ; i++) {
        account = account + 10;
        usleep(50000);  // Wait 50 ms
    }
    return NULL;
}

// Main function
int main(int argc, char* argv[]) {
    int i;          // Iterator

    pthread_t thread[N_THREAD];    // Array of threads

    int iret[N_THREAD];            // Array of thread return values

    // Creating threads
    for (i = 0 ; i < N_THREAD ; i++) {
        iret[i] = pthread_create(&thread[i], NULL, deposit, NULL);
    }

    // Wait until threads are all complete
    for (i = 0 ; i < N_THREAD ; i++) {
        pthread_join(thread[i], NULL);
    }
    printf("Account/N_THREAD(%d): %d\n", N_THREAD, account/N_THREAD);
    return 0;
}
