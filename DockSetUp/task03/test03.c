#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "cond_var.h"

#define THREADS 8
#define SIGNAL_THREADS 3

int counter = 0;
ticket_lock lock;
condition_variable cv;
int threads_waiting = 0;

void *waiter_thread(void *arg)
{
    int id = *(int *)arg;
    ticketlock_acquire(&lock);
    threads_waiting++;
    condition_variable_wait(&cv, &lock); // wait on condition var
    counter++;                           // thread woke up, increment counter
    ticketlock_release(&lock);           // release lock

    return NULL;
}

void *signaler_thread(void *arg)
{
    usleep(100000); // sleep --> 100ms

    for (int i = 0; i < SIGNAL_THREADS; i++)
    {
        condition_variable_signal(&cv);
        usleep(50000); // sleep --> 50ms
    }

    usleep(100000);                    // sleep --> 100ms
    condition_variable_broadcast(&cv); // wake up rest of waiting threads

    return NULL;
}
//--------------------------------------------------------------------//
int main(void)
{
    ticketlock_init(&lock);
    condition_variable_init(&cv);
    pthread_t waiters[THREADS];
    pthread_t signaler;
    int ids[THREADS];

    for (int i = 0; i < THREADS; i++)
    { // create threads to wait
        ids[i] = i + 1;
        pthread_create(&waiters[i], NULL, waiter_thread, &ids[i]);

        usleep(10000); // add delay --> 10ms
    }

    pthread_create(&signaler, NULL, signaler_thread, NULL); // make threads to signal
    pthread_join(signaler, NULL);                           // waits for signaler to terminate

    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(waiters[i], NULL); // waits for waiter threads to terminate
    }

    if (counter != THREADS)
    {
        printf("Value is %d, expected %d\n", counter, THREADS);
        fprintf(stderr, "Failed!"); // error, we failed
    }
    else
    {
        printf("All %d threads woke up :)\n", THREADS);
    }

    return 0;
}
//----------------------------------End of File----------------------------------//