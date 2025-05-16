#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "cp_pattern.h"

static ticket_lock print_lock;
SharedData shared;

void init_shared_data() {
    shared.queue_size = 0;
    shared.total_produced = 0;
    shared.producers_done = false;
    shared.consumers_done = false;
    semaphore_init(&shared.queue_items, 0);
    ticketlock_init(&shared.lock);

    for (int i = 0; i < MAX_NUM; i++) {
        shared.produced[i] = false;
    }
}

//-------------------------------------------------------------------------------//

void* producer(void* arg) {
    long thread_id = (long)arg;

    while(1) {
        int num = rand() % MAX_NUM;

        ticketlock_acquire(&shared.lock);
        //check if the number is already in the queue
        if (shared.total_produced >= MAX_NUM) {
            ticketlock_release(&shared.lock);
            break;
        }

        //if not, enqueue
        if (!shared.produced[num]) {
            shared.produced[num] = true;
            shared.queue[shared.queue_size++] = num;
            shared.total_produced++;

            ticketlock_release(&shared.lock);

            // print
            char msg[128];
            snprintf(msg, sizeof(msg), "Producer %ld generated number: %d", thread_id, num);
            print_msg(msg);

            semaphore_signal(&shared.queue_items);
        } else {
            ticketlock_release(&shared.lock);
        }
    }
    return NULL;
}

//-------------------------------------------------------------------------------//

void* consumer(void* arg) {
    long thread_id = (long)arg;//get thread id

    while (1) {
        semaphore_wait(&shared.queue_items);
        ticketlock_acquire(&shared.lock);        

        if (shared.queue_size == 0 && shared.producers_done) {
            ticketlock_release(&shared.lock);
            break;
        }

        int num = shared.queue[--shared.queue_size];

        ticketlock_release(&shared.lock); // unlock --> other threads can access the queue

        // check divisible by 6?
        bool divisible = (num % 6 == 0);

        //Print to stdout in the following format: Consumer {thread ID} checked {number}. Is it divisible by 6? {result}
        char msg[128];
        snprintf(msg, sizeof(msg), "Consumer %ld checked %d. Is it divisible by 6? %s",
                 thread_id, num, divisible ? "True" : "False");
        print_msg(msg);
    }

    // consumers done
    ticketlock_acquire(&shared.lock);
    shared.consumers_done = true;
    ticketlock_release(&shared.lock);

    return NULL;
}

//-------------------------------------------------------------------------------//

//check if consumers is empty
bool consumers_is_empty() {
    ticketlock_acquire(&shared.lock);
    bool empty = (shared.queue_size == 0);
    ticketlock_release(&shared.lock);
    return empty;
}

//-------------------------------------------------------------------------------//

void start_consumers_producers(int consumers, int producers, int seed) {
    //initialize struct
    init_shared_data();

    srand(seed);
    ticketlock_init(&shared.lock);
    

    pthread_t prod_threads[producers];
    pthread_t cons_threads[consumers];

    //create producer threads
    for (long i = 0; i < producers; i++) {
        pthread_create(&prod_threads[i], NULL, producer, (void*)i);
    }

    //create consumer threads
    for (long i = 0; i < consumers; i++) {
        pthread_create(&cons_threads[i], NULL, consumer, (void*)i);
    }

    //wait for producer to terminate
    for (int i = 0; i < producers; i++) {
        pthread_join(prod_threads[i], NULL);
    }

    ticketlock_acquire(&shared.lock);
    shared.producers_done = true; //producers are done
    ticketlock_release(&shared.lock);

    // Signal consumes to unblock in case they're waiting
    for (int i = 0; i < consumers; i++) {
        semaphore_signal(&shared.queue_items);
    }

    //wait for consumers to terminate
    for (int i = 0; i < consumers; i++) {
        pthread_join(cons_threads[i], NULL);
    }
}

//-------------------------------------------------------------------------------//

void stop_consumers() {
    while(1) {
        ticketlock_acquire(&shared.lock);
        //if consumers done -> break
        if (shared.consumers_done) {
            ticketlock_release(&shared.lock); 
            break;
        }

        // if not, unlock and check again (busy wait)
        ticketlock_release(&shared.lock);
    }
}

//-------------------------------------------------------------------------------//

void print_msg(const char* msg) {
    ticketlock_acquire(&print_lock);
    printf("%s\n", msg);
    ticketlock_release(&print_lock);
}

//-------------------------------------------------------------------------------//

void wait_until_producers_produced_all_numbers() {
    while(1) {
        ticketlock_acquire(&shared.lock);
        //if producers produced every #
        if (shared.total_produced >= MAX_NUM) {
            ticketlock_release(&shared.lock); //unlock and break
            break;
        }
        ticketlock_release(&shared.lock); //if not unlock and check again
    }
}

//-------------------------------------------------------------------------------//

void wait_consumers_queue_empty() {
    while(1) {
        //if queue is empty and produecers are done then break
        if (consumers_is_empty() && shared.producers_done){ 
            break;
        }

        //if not check again
    }
}

//-------------------------------------------------------------------------------//

int main(int argc, char* argv[]) {

    ticketlock_init(&print_lock);

    //Parsing arguments
    int consumers = atoi(argv[1]);
    int producers = atoi(argv[2]);
    int seed = atoi(argv[3]);

    //func calls
    start_consumers_producers(consumers, producers, seed);
    wait_until_producers_produced_all_numbers();
    wait_consumers_queue_empty();
    stop_consumers();

    return 0;
}

//----------------------------------End of File----------------------------------//
