#include "tl_semaphore.h"
#include "ticket_lock.h"
#include <stdio.h>

/*
 * TODO: Implement semaphore_init for the Ticket Lock semaphore.
 */
void semaphore_init(semaphore* sem, int initial_value) {
    // TODO: Define the structure and initialize the semaphore.
    sem->value = initial_value;
    ticketlock_init(&(sem->lock));
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement semaphore_wait using the Ticket Lock mechanism.
 */
void semaphore_wait(semaphore* sem) {
    // TODO: Obtain a ticket and wait until it is your turn; then decrement the semaphore value.
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
/*
 * TODO: Implement semaphore_signal using the Ticket Lock mechanism.
 */
void semaphore_signal(semaphore* sem) {
    // TODO: Increment the semaphore value and allow the next ticket holder to proceed.
      ticketlock_acquire(&(sem->lock)); // lock first
      printf("Has lock\n");
  
      sem->value++; // increment
      printf("Inc\n");
  
      ticketlock_release(&(sem->lock)); // unlocks
}
//----------------------------------End of File----------------------------------//
