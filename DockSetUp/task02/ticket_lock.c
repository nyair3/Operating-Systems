#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include "ticket_lock.h"

void ticketlock_init(ticket_lock *lock)
{
    // initialize ticket and current ticket
    atomic_init(&lock->ticket, 0);
    atomic_init(&lock->cur_ticket, 0);
}

//--------------------------------------------------------------------//

void ticketlock_acquire(ticket_lock *lock)
{
    // get my ticket
    int my_ticket = atomic_fetch_add(&lock->ticket, 1); // wait until it is my turn
    while (atomic_load(&lock->cur_ticket) != my_ticket) // while current ticket is not my ticket #
    {
        sched_yield(); // take thread from running to ready state
    }
}

//--------------------------------------------------------------------//

void ticketlock_release(ticket_lock *lock)
{
    atomic_fetch_add(&lock->cur_ticket, 1); // increment current ticket value by 1
}

//----------------------------------End of File----------------------------------//