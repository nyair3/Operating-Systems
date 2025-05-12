#include "cond_var.h"

void condition_variable_init(condition_variable *cv)
{
    cv->waiters_count = 0; //initialize num waiting threads to 0
    ticketlock_init(&cv->waiters_lock);
    semaphore_init(&cv->sem, 0); 
}
//--------------------------------------------------------------------//
void condition_variable_wait(condition_variable *cv, ticket_lock *ext_lock)
{
    ticketlock_acquire(&cv->waiters_lock); //lock waiters lock
    cv->waiters_count++; //inc. num threads waiting
    ticketlock_release(&cv->waiters_lock); //release waiters lock
    ticketlock_release(ext_lock); //release external lock
    semaphore_wait(&cv->sem); //wait on semaphore
    ticketlock_acquire(ext_lock); //when signaled, release ext lock

}

void condition_variable_signal(condition_variable *cv)
{
    ticketlock_acquire(&cv->waiters_lock); //get waiters lock  
    if (cv->waiters_count > 0) { //if we have threads waiting
        cv->waiters_count--; //Decrement count of waiting threads.
        semaphore_signal(&cv->sem); //wake up thread waiting on semaphore
    }
    ticketlock_release(&cv->waiters_lock); //if no threads waiting release lock
}

//------------------------------------------------------------------------------//

void condition_variable_broadcast(condition_variable *cv)
{
  ticketlock_acquire(&cv->waiters_lock);
    // Wake up all waiting threads
    int waiters = cv->waiters_count;
    for (int i = 0; i < waiters; i++)
    {
        semaphore_signal(&cv->sem);
    }
    cv->waiters_count = 0; //no waiting threads anymore, reset to 0
    ticketlock_release(&cv->waiters_lock); 
}
//----------------------------------End of File----------------------------------//gcc -o test03 ../task03/test03.c ../task03/cond_var.c ../task01/ticket_lock.c ../task01/tl_semaphore.c -lpthread
