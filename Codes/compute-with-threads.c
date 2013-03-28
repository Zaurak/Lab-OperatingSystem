/** Lab Subject :
  * Parallel computing using threads.
  * 2 functions :
  *     ~ add (Note that the substraction will be handle by add too)
  *     ~ mul (Multiplication. Division is not handled by a thread)
  * 4 threads :
  *     ~ (a + b)
  *     ~ (c * d)
  *     ~ (e - f)
  *     ~ (g + h)
  * => (a+b) - (c*d)/(e-f) + (g+h)
  */

#include <stdio.h>      // Input/Output
#include <pthread.h>    // Threads management
#include <sys/time.h>   // Time management

// Define a struct to contain 2 operands in one variable
// (Because we can only send one variable to a thread)
typedef struct operands {
    int a, b;       // Store the operands
    int ret;        // Store the return value
}t_operands;

// Handle addition and substraction
void* add(void* operands) {
    t_operands* ope = (t_operands*) operands;   // Parameter cast
    ope->ret = ope->a + ope->b;                 // Record the result
    return NULL;
}

// Handle addition and substraction
void* mul(void* operands) {
    t_operands* ope = (t_operands*) operands;   // Parameter cast
    ope->ret = ope->a * ope->b;                 // Record the result
    return NULL;
}

// Main function
int main(int argc, char* argv[]) {
    int i;          // Iterator

    int result;     // Store the result

    pthread_t thread[4];    // Array of threads

    int iret[4];            // Array of thread return values

    // Parameters
    int a = 5, b = 4, c = 3, d = 2, e = 0, f = 6, g = 7, h = 8;

    t_operands params[4];

    params[0].a = a;    params[0].b =   b;
    params[1].a = c;    params[1].b =   d;
    params[2].a = e;    params[2].b = - f;
    params[3].a = g;    params[3].b =   h;

    // Iteration loop (to have significant value for Execution time)
    // Creating threads
    iret[0] = pthread_create(&thread[0], NULL,
        add, (void*) &params[0]);
    iret[1] = pthread_create(&thread[1], NULL,
        mul, (void*) &params[1]);
    iret[2] = pthread_create(&thread[2], NULL,
        add, (void*) &params[2]);
    iret[3] = pthread_create(&thread[3], NULL,
        add, (void*) &params[3]);

    // Wait until threads are all complete
    for (i = 0 ; i < 4 ; i++) {
        pthread_join(thread[i], NULL);
        printf("Thread %d returned %d\n", i, iret[i]);
    }

    if (params[2].ret != 0) {
        result = params[0].ret
            - params[1].ret/params[2].ret
            + params[3].ret;
        printf("Result: %d\n", result);
        return 0;
    } else {
        printf("Error: Can't divide by 0 (e[%d] - f[%d])\n", e, f);
        return -1;
    }
}
