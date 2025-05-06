#include "ticket_lock.h"
#include "tl_semaphore.h"
#include <stdio.h>
#define N 2

semaphore sem;

void test_func(void *arg)
{
    semaphore *sem = (semaphore *)arg;
    semaphore_wait(sem);
    // Critical Section up to N threads
    printf("Thread in critical section\n");
    semaphore_signal(sem);
    
}

int main()
{
    semaphore_init(&sem, N);

    // You'll need to create threads here if you want to run test_func concurrently
    // Otherwise, just test it directly:
    test_func(&sem);

    return 0;
}

/*

#include <stdio.h>
#include <pthread.h>
#include "ticket_lock.h"
#include "tl_semaphore.h"

int main()
{
#define N 100
    int x = 0;
    ticket_lock lock;
    ticketlock_init(&lock);
    pthread_t threads[N];
    for (int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, thread_func, &lock);
    }
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("Final x = %d\n", x);
    return 0;
}
//----------------------------------End of File----------------------------------//
*/