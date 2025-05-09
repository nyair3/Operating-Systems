#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include "spin_lock.h"

void spinlock_init(atomic_flag *lock)
{
    atomic_flag_clear(lock);
}

//--------------------------------------------------------------------//

void spinlock_acquire(atomic_flag *lock)
{
    while (atomic_flag_test_and_set(lock))
    {
        // scheduler – yield (i.e. skip)
        sched_yield();
    }
}

//--------------------------------------------------------------------//

void spinlock_release(atomic_flag *lock)
{
    atomic_flag_clear(lock);
} 
//----------------------------------End of File-----------------------//