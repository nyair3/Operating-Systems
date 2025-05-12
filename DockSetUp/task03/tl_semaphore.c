#include "tl_semaphore.h"
#include "ticket_lock.h"
#include <stdio.h>

void semaphore_init(semaphore* sem, int initial_value) {
    sem->value = initial_value;
    ticketlock_init(&(sem->lock));
}
//--------------------------------------------------------------------//

void semaphore_wait(semaphore* sem) {
    while (1)
    {
        ticketlock_acquire(&(sem->lock)); // lock first

        if (sem->value > 0)
        {
            sem->value--;                   // decrement
            ticketlock_release(&(sem->lock)); // unlock
            break;                          // success, exit loop
        }

        ticketlock_release(&(sem->lock)); // unlock if couldn't decrement
        // Busy wait: try again
    }
}
//--------------------------------------------------------------------//

void semaphore_signal(semaphore* sem) {
      ticketlock_acquire(&(sem->lock)); // lock first
  
      sem->value++; // increment
  
      ticketlock_release(&(sem->lock)); // unlocks
}
//----------------------------------End of File----------------------------------//
