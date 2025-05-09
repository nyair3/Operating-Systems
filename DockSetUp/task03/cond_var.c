#include "cond_var.h"

/*
 * TODO: Implement condition_variable_init.
 */
void condition_variable_init(condition_variable *cv)
{
    // TODO: Initialize internal fields.
    cv->waiters_count = 0;
    ticketlock_init(&cv->waiters_lock);
    semaphore_init(&cv->sem, 0); 
}

/*
 * TODO: Implement condition_variable_wait.
 */
void condition_variable_wait(condition_variable *cv, ticket_lock *ext_lock)
{
    // TODO: Increase waiter count, release ext_lock, wait until signaled, then reacquire ext_lock.
     ticketlock_acquire(&cv->waiters_lock);
    cv->waiters_count++;
    ticketlock_release(&cv->waiters_lock);
    
    ticketlock_release(ext_lock);
    
    semaphore_wait(&cv->sem);
    
    ticketlock_acquire(ext_lock);
}

/*
 * TODO: Implement condition_variable_signal.
 */
void condition_variable_signal(condition_variable *cv)
{
    // TODO: Signal one waiting thread.
    ticketlock_acquire(&cv->waiters_lock);
    if (cv->waiters_count > 0) {
        
        cv->waiters_count--;
        semaphore_signal(&cv->sem); 
    }
    ticketlock_release(&cv->waiters_lock);
}

/*
 * TODO: Implement condition_variable_broadcast.
 */
void condition_variable_broadcast(condition_variable *cv)
{
    // TODO: Signal all waiting threads.
  ticketlock_acquire(&cv->waiters_lock);
    // Wake up all waiting threads
    int waiters = cv->waiters_count;
    for (int i = 0; i < waiters; i++) {
        semaphore_signal(&cv->sem);
    }
    cv->waiters_count = 0;
    ticketlock_release(&cv->waiters_lock);
}
//----------------------------------End of File----------------------------------//gcc -o test03 ../task03/test03.c ../task03/cond_var.c ../task01/ticket_lock.c ../task01/tl_semaphore.c -lpthread
