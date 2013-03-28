/** Lab Subject :
  * Parallel computing using processes.
  * 4 child processes (+1 parent process) :
  *     ~ (a + b)
  *     ~ (c * d)
  *     ~ (e - f)
  *     ~ (g + h)
  * => (a+b) - (c*d)/(e-f) + (g+h)
  *
  * (P)
  *  |__(1)
  *  |   |__(2)
  *  |   |   |__(3)
  *  |  a+b  |   |__(4)
  *  |   |  g+h  |   |
  *  |   |   |  c*d e-f
  *  |   |   |   |___|
  *  |   |   |___|
  *  |   |___|
  *  |___|
  *  |
  */

#include <stdlib.h>     // Useful for exit() function
#include <stdio.h>      // Input/Output
#include <unistd.h>     // Useful for fork() function
#include <sys/types.h>  // System utilities...
#include <sys/shm.h>    //  ...
#include <sys/wait.h>   //  ...

#define KEY 4567
#define PERMS 0660	// Define an rw- permission for user and group.

int main(int argc, char* argv[]) {
    int	id;             // Id of the shared memory segment
    int* shared_data;   // Pointer to the shared memory
    int	result;         // "Local" result variable (Process dependant)

    // Parameters
    int a = 5, b = 4, c = 3, d = 2, e = 0, f = 6, g = 7, h = 8;

    // Allocate a new int sized shared memory segment and keep its id
    id = shmget(KEY, sizeof(int), IPC_CREAT | PERMS);

    // Get the access to the memory allocated previously
    shared_data = (int*) shmat(id, NULL, 0);

    // Start of a fork
    if (fork() == 0) {  // First Child process

        printf("Child process 1: "
            "Child1.res = (a+b)\n");
        result = a + b;

        if (fork() == 0) {  // Second Child process
            printf("Child process 2: "
                "Child2.res = (g+h)\n");
            result = g + h;

            if (fork() == 0) {  // Third Child process
                printf("Child process 3: "
                    "Child3.res = (c*d)\n");
                result = c * d;

                if (fork() == 0) {  // Fourth Child process
                    printf("Child process 4: "
                        "shared_data = (e-f)\n");
                    *(shared_data) = e - f;
                    exit(0);
                }   // End of Child 4

                wait(NULL);
                printf("Child process 3: "
                    "shared_data = - Child3.res / shared_data\n");
                *(shared_data) = - *(shared_data) / result;
                exit(0);
            } // End of Child 3
            wait(NULL);
            printf("Child process 2: "
                "shared_data = Child2.res + shared_data\n");
            *(shared_data) = *(shared_data) + result;
            exit(0);
        } // End of Child 2
        wait(NULL);
        printf("Child process 1: "
                "shared_data = Child1.res + shared_data\n");
        *(shared_data) = *(shared_data) + result;
        exit(0);
    } else {// End of Child 1
        // Parent Process
        printf("Parent process: Waiting end of child process 1...\n");

        wait(NULL);	// Wait the termination of the first child process

        printf("Result: %d\n", *(shared_data));

        // Mark the segment to be destroyed : Free the shared memory
        shmctl(id, IPC_RMID, NULL);
    }
    return 0;
}
