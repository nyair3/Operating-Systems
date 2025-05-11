#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "cond_var.h"

#define THREADS 8
#define SIGNAL_THREADS 3 // Number of threads to wake with individual signals

int counter = 0;
ticket_lock lock;
condition_variable cv;
int threads_waiting = 0;

// Thread function for waiter threads
void *waiter_thread(void *arg)
{
    int id = *(int *)arg;

    // Acquire the lock
    ticketlock_acquire(&lock);

    printf("Thread %d is waiting\n", id);
    threads_waiting++;

    // Wait on the condition variable
    condition_variable_wait(&cv, &lock);

    // After waking up, increment the counter
    counter++;
    printf("Thread %d woke up, counter = %d\n", id, counter);

    // Release the lock
    ticketlock_release(&lock);

    return NULL;
}
//--------------------------------------------------------------------//
// Thread function for the signaler
void *signaler_thread(void *arg)
{
    // Sleep briefly to ensure waiter threads are waiting
    usleep(100000); // 100ms

    // Signal individual threads
    for (int i = 0; i < SIGNAL_THREADS; i++)
    {
        printf("Signaling one thread\n");
        condition_variable_signal(&cv);
        usleep(50000); // 50ms between signals
    }

    // Sleep briefly before broadcast
    usleep(100000); // 100ms

    // Broadcast to wake up all remaining threads
    printf("Broadcasting to all remaining threads\n");
    condition_variable_broadcast(&cv);

    return NULL;
}
//--------------------------------------------------------------------//
int main(void)
{
    // Initialize lock and condition variable
    ticketlock_init(&lock);
    condition_variable_init(&cv);

    pthread_t waiters[THREADS];
    pthread_t signaler;
    int ids[THREADS];

    // Create waiter threads
    for (int i = 0; i < THREADS; i++)
    {
        ids[i] = i + 1;
        pthread_create(&waiters[i], NULL, waiter_thread, &ids[i]);

        // Small delay to ensure threads start in order
        usleep(10000); // 10ms
    }

    // Create signaler thread
    pthread_create(&signaler, NULL, signaler_thread, NULL);

    // Join all threads
    pthread_join(signaler, NULL);
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(waiters[i], NULL);
    }

    // Verify results
    if (counter != THREADS)
    {
        printf("Value is %d, expected %d\n", counter, THREADS);
        fprintf(stderr, "Failed!");
    }
    else
    {
        printf("Test passed! All %d threads woke up.\n", THREADS);
    }

    return 0;
}
//----------------------------------End of File----------------------------------//