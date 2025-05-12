#include "tas_semaphore.h"
#include "spin_lock.h"
#include <stdio.h>

void semaphore_init(semaphore *sem, int initial_value)
{
    sem->value = initial_value;  // Set the starting value
    spinlock_init(&(sem->lock)); // Initialize the spinlock
}

//--------------------------------------------------------------------//

void semaphore_wait(semaphore *sem)
{
    while (1)
    {
        spinlock_acquire(&(sem->lock)); // lock first
        if (sem->value > 0)
        {
            sem->value--;                   // decrement
            spinlock_release(&(sem->lock)); // unlock
            break;                          // success so exit loop
        }
        spinlock_release(&(sem->lock)); // unlock if couldn't decrement
    }
}

//--------------------------------------------------------------------//

void semaphore_signal(semaphore *sem)
{
    spinlock_acquire(&(sem->lock));
    sem->value++; // increment
    spinlock_release(&(sem->lock));
}

//----------------------------------End of File----------------------------------//