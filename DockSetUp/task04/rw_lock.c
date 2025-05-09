#include "rw_lock.h"

/*
 * TODO: Implement rwlock_init.
 */
void rwlock_init(rwlock* lock) {
    // TODO: Initialize the lock structure.
    atomic_init(&lock->readers, 0);
    atomic_init(&lock->writer, 0);
    atomic_init(&lock->writer_waiting, 0);
}

/*
 * TODO: Implement rwlock_acquire_read.
 */
void rwlock_acquire_read(rwlock* lock) {
    // TODO: Allow multiple readers while ensuring no writer is active.

     while (1) {
        if (atomic_load(&lock->writer) || atomic_load(&lock->writer_waiting) > 0) {
            continue;
        }
        
        atomic_fetch_add(&lock->readers, 1);
        
        if (atomic_load(&lock->writer) == 0) {
            break; 
        }
        
        atomic_fetch_sub(&lock->readers, 1);
    }
    /*
        rwlock_acquire_read(&rw);
        Readers section
        if(not synced condition){
        rwlock_acquire_upgrade_reader(&rw);Writer section
        rwlock_release_write(&rw);
        }
        else{
        rwlock_release_read(&rw);
        }
    */
}

/*
 * TODO: Implement rwlock_release_read.
 */
void rwlock_release_read(rwlock* lock) {
    // TODO: Decrement the reader count.
    atomic_fetch_sub(&lock->readers, 1);
}

/*
 * TODO: Implement rwlock_acquire_write.
 */
void rwlock_acquire_write(rwlock* lock) {
    // TODO: Ensure exclusive access for writing.
     atomic_fetch_add(&lock->writer_waiting, 1);
    
    // Wait for no readers and no active writer
    while (1) {
        // Check if there are no readers and no active writer
        if (atomic_load(&lock->readers) == 0) {
            // Try to acquire write lock
            int expected = 0;
            if (atomic_compare_exchange_strong(&lock->writer, &expected, 1)) {
                break; // Success
            }
        }
        // Busy wait
    }
    
    // No longer waiting (we're now active)
    atomic_fetch_sub(&lock->writer_waiting, 1);
}


/*
 * TODO: Implement rwlock_release_write.
 */
void rwlock_release_write(rwlock* lock) {
    // TODO: Release the write lock.
        atomic_store(&lock->writer, 0);

}
