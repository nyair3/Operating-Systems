#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include "spin_lock.h"

//
//
//
    // DEAL WITH PTHREAD

//
//
//

/*
int main()
{
#define N 2
    atomic_flag lock;
    spinlock_init(&lock);
    pthread_t threads[N];
    for (int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL,
                       thread_func, &lock);
    }
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
    */

//--------------------------------------------------------------------\\

void spinlock_init(atomic_flag *lock)
{
    atomic_flag_clear(lock);
}

//--------------------------------------------------------------------\\

void spinlock_acquire(atomic_flag *lock)
{
    while (atomic_flag_test_and_set(lock))
    {
        // scheduler – yield (i.e. skip)
        sched_yield();
    }
}

//--------------------------------------------------------------------\\

void spinlock_release(atomic_flag *lock)
{
    atomic_flag_clear(lock);
}

//--------------------------------------------------------------------\\


void thread_func(void *arg)
{
    atomic_flag *lock = (atomic_flag *)arg;
    spinlock_acquire(lock); // critical section
    printf("Thread %lu in CS\n",
           pthread_self());
    spinlock_release(lock);
}

//----------------------------------End of File----------------------------------\\