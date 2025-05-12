#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "tl_semaphore.h"

#define THREADS 16

int x = 0;

struct args
{
    semaphore *sem1;
    semaphore *sem2;
};

//------------------------------------------------------------------------//

void *thread_function(void *arg)
{
    struct args *args = (struct args *)arg;
    semaphore_wait(args->sem1);
    semaphore_wait(args->sem2);
    x++;
    semaphore_signal(args->sem2);
    semaphore_signal(args->sem1);
    return NULL;
}

//------------------------------------------------------------------------//

int main(void)
{
    // initialize 2 semaphores
    semaphore sem1, sem2;
    semaphore_init(&sem1, 1);
    semaphore_init(&sem2, 1);

    pthread_t threads[THREADS];
    struct args args[THREADS];

    for (int i = 0; i < THREADS; i++) // create threads
    {
        args[i].sem1 = &sem1;
        args[i].sem2 = &sem2;
        pthread_create(&threads[i], NULL, thread_function, (void *)&args[i]);
    }

    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL); // waits for threads at i to terminate
    }

    if (x != THREADS) // if x isnt equal to the set number of threads we failed
    {
        printf("Value is %d, expected %d\n", x, THREADS);
        fprintf(stderr, "Failed!"); // error
    }
    printf("Num threads is: %d\n", x);
    return 0;
}

//----------------------------------End of File----------------------------------//
