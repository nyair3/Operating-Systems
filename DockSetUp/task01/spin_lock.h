#include <stdatomic.h>

void spinlock_init(atomic_flag *lock);

void spinlock_acquire(atomic_flag *lock);

void spinlock_release(atomic_flag *lock);

void thread_func(void *arg);

