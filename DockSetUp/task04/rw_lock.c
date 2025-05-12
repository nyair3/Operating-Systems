#include "rw_lock.h"

void rwlock_init(rwlock *lock)
{
    semaphore_init(&lock->mutex, 1); 
    semaphore_init(&lock->write_lock, 1);
    lock->num_readers = 0; //no readers yet
}

//--------------------------------------------------------------------//

void rwlock_acquire_read(rwlock *lock)
{
    semaphore_wait(&lock->mutex); //lock the mutex

    if(lock->num_readers == 0){ //if no readers 
        semaphore_wait(&lock->write_lock); //writer waits
    }

    lock->num_readers++;
    semaphore_signal(&lock->mutex);
}

//--------------------------------------------------------------------//

void rwlock_release_read(rwlock *lock)
{
    semaphore_wait(&lock->mutex); //lock the mutex
    lock->num_readers--; //decrement num readers
    if(lock->num_readers==0){ //if no readers
        semaphore_signal(&lock->write_lock); //wake up writer
    }
    semaphore_signal(&lock->mutex); //signal mutex
}

//--------------------------------------------------------------------//

void rwlock_acquire_write(rwlock* lock) {
    semaphore_wait(&lock->write_lock); 
}

//--------------------------------------------------------------------//

void rwlock_release_write(rwlock *lock)
{
    semaphore_signal(&lock->write_lock);
}

//----------------------------------End of File----------------------------------//
