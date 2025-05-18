#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "local_storage.h"
#include "cond_var.h"
#include <stdlib.h>
#define THREADS 100



void *thread_function(void *arg)
{
    condition_variable *cv = (condition_variable*)arg;
    ticket_lock tl;
    ticketlock_init(&tl);
    ticketlock_acquire(&tl);
    condition_variable_wait(cv, &tl);

    sched_yield();
    tls_thread_alloc();
    sched_yield();
    set_tls_data((void*)pthread_self());

    sched_yield();
    if ((pthread_t)get_tls_data() != pthread_self()) {
        printf("Shouldn't happen, failure");
        exit(1);
    }

    sched_yield();
    tls_thread_free();

    ticketlock_release(&tl);
    return NULL;
}
//--------------------------------------------------------------------//

int main(void)
{
    condition_variable cond;
    condition_variable_init(&cond);
    init_storage();

    pthread_t threads[THREADS];

    printf("Preparing all threads\n");

    for(int i = 0; i < THREADS; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, &cond);
    }

    printf("Prepared all threads. Waiting until all threads reach starting point...\n");
    sleep(1);

    printf("Triggering stress in 1 second!\n");
    sleep(1);

    condition_variable_broadcast(&cond);

    for(int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("All threads are done, test success\n");
    return 0;
}
//----------------------------------End of File----------------------------------//
/// TSVI Comments
    // check if the thread exists and is the current thread

    //-1 says its free/empty to use
    // find an empty/free thread and assign it to me

    // if full print