#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "rw_lock.h"

#define NUM_READERS 5
#define NUM_WRITERS 3
#define READS_PER_READER 100
#define WRITES_PER_WRITER 50

// Shared data protected by the lock
int shared_counter = 0;
rwlock lock;

// Flag to detect inconsistencies
int inconsistency_detected = 0;

void* reader_thread(void* arg) {
    int thread_id = *(int*)arg;
    int local_reads = 0;
    
    for (int i = 0; i < READS_PER_READER; i++) {
        rwlock_acquire_read(&lock);
        
        // Read operation (just reading the current value)
        int value = shared_counter;
        
        // Simulate some work
        usleep(rand() % 1000);
        
        // Verify the value didn't change during our read
        if (value != shared_counter) {
            printf("Reader %d detected inconsistency! %d != %d\n", 
                   thread_id, value, shared_counter);
            inconsistency_detected = 1;
        }
        
        rwlock_release_read(&lock);
        local_reads++;
        
        // Small delay between operations
        usleep(rand() % 500);
    }
    
    printf("Reader %d completed %d reads\n", thread_id, local_reads);
    return NULL;
}

void* writer_thread(void* arg) {
    int thread_id = *(int*)arg;
    int local_writes = 0;
    
    for (int i = 0; i < WRITES_PER_WRITER; i++) {
        rwlock_acquire_write(&lock);
        
        // Write operation (incrementing the counter)
        shared_counter++;
        
        // Simulate some work
        usleep(rand() % 2000);
        
        rwlock_release_write(&lock);
        local_writes++;
        
        // Small delay between operations
        usleep(rand() % 1000);
    }
    
    printf("Writer %d completed %d writes\n", thread_id, local_writes);
    return NULL;
}

int main() {
    // Initialize random number generator
    srand(time(NULL));
    
    // Initialize the read-write lock
    rwlock_init(&lock);
    
    // Create thread IDs and threads
    pthread_t reader_threads[NUM_READERS];
    pthread_t writer_threads[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    
    // Start reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&reader_threads[i], NULL, reader_thread, &reader_ids[i]);
    }
    
    // Start writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i;
        pthread_create(&writer_threads[i], NULL, writer_thread, &writer_ids[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(reader_threads[i], NULL);
    }
    
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writer_threads[i], NULL);
    }
    
    // Check results
    int expected_final_count = NUM_WRITERS * WRITES_PER_WRITER;
    printf("\n--- Test Results ---\n");
    printf("Final counter value: %d\n", shared_counter);
    printf("Expected counter value: %d\n", expected_final_count);
    
    if (shared_counter != expected_final_count) {
        printf("TEST FAILED: Counter values don't match!\n");
        return 1;
    }
    
    if (inconsistency_detected) {
        printf("TEST FAILED: Readers detected inconsistencies!\n");
        return 1;
    }
    
    printf("TEST PASSED: Read-write lock implementation works correctly!\n");
    return 0;
}