#include "spin_lock.c"
#include "tas_semaphore.c"

semaphore sem;
semaphore_init(&sem, N);
void test_func(void *arg)
{
    semaphore *sem = (semaphore *)arg;
    semaphore_wait(sem);
    Critical Section up to N threads
        semaphore_signal(sem);
}
//----------------------------------End of File----------------------------------\\