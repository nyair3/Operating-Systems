#include <stdatomic.h>
#ifndef TICKET_LOCK_H
#define TICKET_LOCK_H

typedef struct
{
 atomic_int ticket ;
 atomic_int cur_ticket ;
} ticket_lock ;

//--------------------------------------------------------------------//
void ticketlock_init(ticket_lock *lock);

//--------------------------------------------------------------------//

void ticketlock_acquire(ticket_lock *lock);

//--------------------------------------------------------------------//

void ticketlock_release(ticket_lock *lock);

//--------------------------------------------------------------------//

void thread_func(void *arg);

//--------------------------------------------------------------------//

#endif
//----------------------------------End of File----------------------------------//