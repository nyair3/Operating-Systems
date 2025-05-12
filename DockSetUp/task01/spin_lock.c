#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include "spin_lock.h"

void spinlock_init(atomic_flag *lock)
{
    atomic_flag_clear(lock); //initialize the spin lock
}

//--------------------------------------------------------------------//

void spinlock_acquire(atomic_flag *lock)
{
    while (atomic_flag_test_and_set(lock)) //when we have the lock
    {
        sched_yield(); //thread goes into ready state
    }
}

//--------------------------------------------------------------------//

void spinlock_release(atomic_flag *lock)
{
    atomic_flag_clear(lock); //releases the lock
} 
//----------------------------------End of File-----------------------//