#include "tl_semaphore.h"
#include "ticket_lock.h"
#include <stdio.h>

void semaphore_init(semaphore *sem, int initial_value)
{
    sem->value = initial_value;
    ticketlock_init(&(sem->lock));
}
//--------------------------------------------------------------------//

void semaphore_wait(semaphore *sem)
{
    while (1)
    {
        ticketlock_acquire(&(sem->lock));
        if (sem->value > 0)
        {
            sem->value--;                     // decrement
            ticketlock_release(&(sem->lock)); // unlock
            break;                            // success so exit loop
        }

        ticketlock_release(&(sem->lock)); // unlock if couldn't decrement
    }
}
//--------------------------------------------------------------------//

void semaphore_signal(semaphore *sem)
{
    ticketlock_acquire(&(sem->lock));
    sem->value++; // increment
    ticketlock_release(&(sem->lock));
}
//----------------------------------End of File----------------------------------//