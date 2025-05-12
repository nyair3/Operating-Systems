#include "tas_semaphore.h"
#include "spin_lock.h"
#include <stdio.h>

/*
 * TODO: Implement semaphore_init using a TAS spinlock.
 */
void semaphore_init(semaphore *sem, int initial_value)
{
    // TODO: Define the structure and initialize the semaphore.
    sem->value = initial_value;  // Set the starting value
    spinlock_init(&(sem->lock)); // Initialize the spinlock
}

//--------------------------------------------------------------------//

/*
 * TODO: Implement semaphore_wait using the TAS spinlock mechanism.
 */
void semaphore_wait(semaphore *sem)
{
    while (1)
    {
        spinlock_acquire(&(sem->lock)); // lock first

        if (sem->value > 0) 
        {
            sem->value--;                   // decrement
            spinlock_release(&(sem->lock)); // unlock
            break;                          // success, exit loop
        }

        spinlock_release(&(sem->lock)); // unlock if couldn't decrement
        // Busy wait: try again
    }
}

//--------------------------------------------------------------------//

void semaphore_signal(semaphore *sem)
{
    spinlock_acquire(&(sem->lock)); // lock first

    sem->value++; // increment

    spinlock_release(&(sem->lock)); // unlocks
}

//----------------------------------End of File----------------------------------//