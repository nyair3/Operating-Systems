#include <stdatomic.h>
#ifndef TICKET_LOCK_H
#define TICKET_LOCK_H

typedef struct {
    atomic_int next;
    atomic_int now_serving;
} ticket_lock;


void ticketlock_init(ticket_lock *lock);


//--------------------------------------------------------------------//

void ticketlock_acquire(ticket_lock *lock);

//--------------------------------------------------------------------//

void ticketlock_release(ticket_lock *lock);

//--------------------------------------------------------------------//