#include "spin_lock.h"
#include "tas_semaphore.h"
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
#include "spin_lock.h"
#include "tas_semaphore.h"
#include <stdio.h>
#define N 2

semaphore sem;
semaphore_init(&sem, N);
void test_func(void *arg)
{
    semaphore *sem = (semaphore *)arg;
    semaphore_wait(sem);
    //gcc -o DockSetUp spin_lock.c tas_semaphore.c test.c -lpthread
    //Critical Section up to N threads
        semaphore_signal(sem);
}
//----------------------------------End of File----------------------------------//
*/