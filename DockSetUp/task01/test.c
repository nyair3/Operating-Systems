#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "tas_semaphore.h"

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
    struct args *args = (struct args*)arg;
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
    semaphore sem1, sem2;
    semaphore_init(&sem1, 1);
    semaphore_init(&sem2, 1);

    pthread_t threads[THREADS];
    struct args args[THREADS];

    for(int i = 0; i < THREADS; i++)
    {
        args[i].sem1 = &sem1;
        args[i].sem2 = &sem2;
        pthread_create(&threads[i], NULL, thread_function, (void*)&args[i]);
    }

    for(int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    if(x != THREADS)
    {
        printf("Value is %d, expected %d\n", x, THREADS);
        fprintf(stderr, "Failed!");
    }
    return 0;
}
/*#include "spin_lock.h"
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
*/

//----------------------------------End of File----------------------------------//
