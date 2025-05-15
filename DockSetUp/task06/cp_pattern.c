#include "cp_pattern.h"
#include <stdio.h>
#include <stdlib.h>

condition_variable cv;
condition_variable_init(&cv);

/*
 * TODO: Implement start_consumers_producers.
 * This function should:
 *  - Print the configuration (number of consumers, producers, seed).
 *  - Seed the random number generator using srand().
 *  - Create producer and consumer threads.
 */
void start_consumers_producers(int consumers, int producers, int seed) {
    // TODO: Print configuration and start threads.
    pthread_t prods[producers];
    pthread_t cons[consumers];
    strand(1000000); //initializes randomizer
    for(int i = 0; i<1000000; i++){
        producers = rand() % 1000001;
    }
    
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement stop_consumers to stop all consumers threads.
 */
void stop_consumers() {
    // TODO: Stop the consumer thread with the given id.
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement print_msg to perform synchronized printing.
 */
void print_msg(const char* msg) {
    // TODO: Print the message ensuring output does not overlap.
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement wait_until_producers_produced_all_numbers 
 * The function should wait until all numbers between 0 and 1,000,000 have been produced.
 */
void wait_until_producers_produced_all_numbers() {
    // TODO: Wait until production of numbers (0 to 1,000,000) is complete.
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement wait_consumers_queue_empty to wait until queue is empty, 
 * if queue is already empty - return immediately without waiting.
 */
void wait_consumers_queue_empty() {
    // TODO: Return non-zero if the consumer queue is empty.
    return 0;
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement a main function that controls the producer-consumer process
 */
int main(int argc, char* argv[]) {
    // TODO: Parse arguments.
    // TODO: Start producer-consumer process.
    // TODO: Wait for threads to finish and clean up resources.
    start_consumers_producers(); //start consumers producers
    wait_until_producers_produced_all_numbers();
    wait_consumers_queue_empty();
    stop_consumers();
    return 0;
}
//--------------------------------------------------------------------//
bool cons_is_empty()
{
    return false;
}
//----------------------------------End of File----------------------------------//
