#include <stdio.h>
#include <pthread.h>
#include "ticket_lock.h"
#include "tl_semaphore.h"

int main()
{
#define N 100
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