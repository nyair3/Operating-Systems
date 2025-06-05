#ifndef CP_PATTERN_H
#define CP_PATTERN_H
#include "cond_var.h"
#include "tl_semaphore.h"
#include "ticket_lock.h"

#define MAX_NUM 1000000

/*
 * Starts the consumers and producers, and passes the seed to the producers.
 */

 typedef struct {
    int queue[MAX_NUM];
    int queue_size;
    bool produced[MAX_NUM];
    int total_produced;
    bool producers_done;
    bool consumers_done;

    semaphore queue_items;
    ticket_lock lock; 
    condition_variable cv;
} SharedData;

extern SharedData shared;

//--------------------------------------------------------------------//
/*
 * Creates producer threads
 */
void* producer(void* arg);
//--------------------------------------------------------------------//
/*
 * Creates consumers threads
 */
void* consumer(void* arg);
//--------------------------------------------------------------------//
/*
 * Checks if consumers is empty
 */
bool consumers_is_empty();

//--------------------------------------------------------------------//
/*
 * Allocate consumer and producer threads
 */
void start_consumers_producers(int consumers, int producers, int seed);
//--------------------------------------------------------------------//
/*
 * Stops the consumers.
 */
void stop_consumers();
//--------------------------------------------------------------------//
/*
 * Prints a message synchronously to avoid overlapping output.
 */
void print_msg(const char* msg);
//--------------------------------------------------------------------//
/*
 * Waits until all numbers between 0 and 1,000,000 have been produced.
 */
void wait_until_producers_produced_all_numbers();
//--------------------------------------------------------------------//
/*
 * wait until queue is empty, if queue is already empty - return immediately without waiting.
 */
void  wait_consumers_queue_empty();

#endif // CP_PATTERN_H
//----------------------------------End of File----------------------------------//
