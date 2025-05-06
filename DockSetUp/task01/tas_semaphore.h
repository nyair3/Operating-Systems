#ifndef TAS_SEMAPHORE_H
#define TAS_SEMAPHORE_H

#include <stdatomic.h>
#include "spin_lock.h"

/*
 * Define the semaphore type.
 * Write your struct details in this file..
 */

typedef struct {
    int value;         // The semaphore counter
    spinlock lock;     // A spinlock to protect the value
} semaphore;
/*
 * Initializes the semaphore pointed to by 'sem' with the specified initial value.
 */
void semaphore_init(semaphore* sem, int initial_value);

/*
 * Decrements the semaphore (wait operation).
 */
void semaphore_wait(semaphore* sem);

/*
 * Increments the semaphore (signal operation).
 */
void semaphore_signal(semaphore* sem);



#endif // TAS_SEMAPHORE_H

//----------------------------------End of File----------------------------------//