/** Lab Subject :
  * Mutual exclusion enforced by semaphores fixing race condition
  */

#include <stdio.h>      // Input/Output
#include <unistd.h>     // Time management (usleep)
#include <pthread.h>    // Threads management
#include <sys/sem.h>    // Semaphores management

#define N_THREAD 10
#define PERMS 0660  // -rw permissions for group and user

int account = 0;

int semId;

int initSem(int semId, int semNum, int initValue) {
    return semctl(semId, semNum, SETVAL, initValue);
}

/* An operation list is structured like this : 
*   { semphore index, operation, flags }
* The operation is an integer value interpreted like this :
*   >= 0 : Rise the semaphore value by this value. 
*       This trigger the awakening of semaphores waiting for a rise.
*   == 0 : Wait for the semaphore to be at value 0.
*    < 0 : Substract abs(value) to the semaphore. 
*       If then the semaphore is negative, wait for a rise.
*/

// Try to take a resource, wait if not available
int P(int semId, int semNum) {
    // Operation list of 1 operation, taking resource, no flag
    struct sembuf operationList[1];
    operationList[0].sem_num = semNum;
    operationList[0].sem_op = -1;
    operationList[0].sem_flg = 0;

    return semop(semId, operationList, 1);
}

// Release a resource
int V(int semId, int semNum) {
    // Operation list of 1 operation, releasing resource, no flag
    struct sembuf operationList[1];
    operationList[0].sem_num = semNum;
    operationList[0].sem_op = 1;
    operationList[0].sem_flg = 0;
    
    return semop(semId, operationList, 1);
}

void* deposit(void* nothing) {
    int i;
    for (i = 0 ; i < 10 ; i++) {
        P(semId, 0);        // Take resource/semaphore 0 of semID 
        account = account + 10;
        V(semId, 0);        // Release resource/semaphore 0 of semID
        usleep(50*1000);  // Wait 50 ms
    }
    return NULL;
}

// Main function
int main(int argc, char* argv[]) {
    int i;          // Iterator
    
    // We create a set of 1 semaphore    
    // ftok generates a key based on the program name and a char value
    // This avoid to pick an arbitrary key already existing
    semId = semget(ftok(argv[0], 'A'), 1, IPC_CREAT | PERMS);
    
    // Set the semaphore at index 0 to value 1 (= available for use)
    initSem(semId, 0, 1);
    
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
