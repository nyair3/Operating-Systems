#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "rw_lock.h"

// Shared variables
int shared_data = 0;
int writer_accessed_during_read = 0;
rwlock lock;

// Time tracking
time_t reader_start_time = 0;
time_t reader_end_time = 0;
time_t writer_start_time = 0;
time_t writer_end_time = 0;

// Helper function to calculate average duration
double calculate_avg_duration(time_t start_times[], time_t end_times[], int count) {
    double total = 0;
    for (int i = 0; i < count; i++) {
        total += (double)(end_times[i] - start_times[i]);
    }
    return total / count;
}

/******** TEST 1: One reader, one writer ********/

void* test1_reader(void* arg) {
    printf("Reader starting\n");
    reader_start_time = time(NULL);
    
    // Acquire read lock
    rwlock_acquire_read(&lock);
    
    printf("Reader acquired lock, will hold for 15 seconds\n");
    
    // Hold the read lock for 15 seconds
    for (int i = 0; i < 15; i++) {
        printf("Reader reading data: %d (second %d)\n", shared_data, i+1);
        
        // Check if writer modified data while we're reading
        if (shared_data != 0) {
            writer_accessed_during_read = 1;
            printf("ERROR: Writer modified data during read!\n");
        }
        
        sleep(1);
    }
    
    rwlock_release_read(&lock);
    reader_end_time = time(NULL);
    printf("Reader finished after %ld seconds\n", reader_end_time - reader_start_time);
    
    return NULL;
}

void* test1_writer(void* arg) {
    printf("Writer starting\n");
    writer_start_time = time(NULL);
    
    // Try to acquire write lock
    printf("Writer attempting to acquire lock\n");
    rwlock_acquire_write(&lock);
    
    writer_start_time = time(NULL);
    printf("Writer acquired lock after %ld seconds\n", 
           writer_start_time - (reader_start_time + 5));
    
    // Modify the shared data
    shared_data = 100;
    printf("Writer modified data to: %d\n", shared_data);
    
    // Hold the lock for 3 seconds
    sleep(3);
    
    rwlock_release_write(&lock);
    writer_end_time = time(NULL);
    printf("Writer released lock\n");
    
    return NULL;
}

void run_test1() {
    printf("\n===== TEST 1: One reader, one writer =====\n");
    
    // Initialize
    rwlock_init(&lock);
    shared_data = 0;
    writer_accessed_during_read = 0;
    
    pthread_t reader, writer;
    
    // Start reader
    pthread_create(&reader, NULL, test1_reader, NULL);
    
    // Wait 5 seconds, then start writer
    sleep(5);
    pthread_create(&writer, NULL, test1_writer, NULL);
    
    // Wait for threads to finish
    pthread_join(reader, NULL);
    pthread_join(writer, NULL);
    
    // Check results
    printf("\nTest 1 Results:\n");
    if (writer_accessed_during_read) {
        printf("TEST FAILED: Writer accessed resource while reader was active\n");
    } else if (writer_start_time < reader_end_time) {
        printf("TEST FAILED: Writer acquired lock before reader finished\n");
    } else {
        printf("TEST PASSED: No context switch occurred before reader finished\n");
    }
    
    printf("Reader duration: %ld seconds\n", reader_end_time - reader_start_time);
    printf("Writer waited approximately: %ld seconds\n", 
           writer_start_time - (reader_start_time + 5));
}

/******** TEST 2: Multiple readers, one writer ********/

#define NUM_READERS 15

// Tracking for multiple readers
time_t readers_start_times[NUM_READERS];
time_t readers_end_times[NUM_READERS];
int readers_active = 0;

void* test2_reader(void* arg) {
    int id = *(int*)arg;
    printf("Reader %d starting\n", id);
    readers_start_times[id] = time(NULL);
    
    // Acquire read lock
    rwlock_acquire_read(&lock);
    
    __sync_fetch_and_add(&readers_active, 1);
    printf("Reader %d acquired lock, will hold for 15 seconds\n", id);
    
    // Hold the read lock for 15 seconds
    for (int i = 0; i < 15; i++) {
        printf("Reader %d reading data: %d (second %d)\n", id, shared_data, i+1);
        
        // Check if writer modified data while we're reading
        if (shared_data != 0) {
            writer_accessed_during_read = 1;
            printf("ERROR: Writer modified data during read! (detected by reader %d)\n", id);
        }
        
        sleep(1);
    }
    
    rwlock_release_read(&lock);
    __sync_fetch_and_sub(&readers_active, 1);
    readers_end_times[id] = time(NULL);
    printf("Reader %d finished after %ld seconds\n", id, readers_end_times[id] - readers_start_times[id]);
    
    return NULL;
}

void* test2_writer(void* arg) {
    printf("Writer starting\n");
    writer_start_time = time(NULL);
    
    // Try to acquire write lock
    printf("Writer attempting to acquire lock\n");
    rwlock_acquire_write(&lock);
    
    writer_start_time = time(NULL);
    printf("Writer acquired lock after %ld seconds\n", 
           writer_start_time - (readers_start_times[0] + 5));
    
    // Check if any readers are still active
    if (readers_active > 0) {
        printf("ERROR: Writer acquired lock while %d readers are still active!\n", readers_active);
    }
    
    // Modify the shared data
    shared_data = 100;
    printf("Writer modified data to: %d\n", shared_data);
    
    // Hold the lock for 3 seconds
    sleep(3);
    
    rwlock_release_write(&lock);
    writer_end_time = time(NULL);
    printf("Writer released lock\n");
    
    return NULL;
}

void run_test2() {
    printf("\n===== TEST 2: %d readers, one writer =====\n", NUM_READERS);
    
    // Initialize
    rwlock_init(&lock);
    shared_data = 0;
    writer_accessed_during_read = 0;
    readers_active = 0;
    
    pthread_t readers[NUM_READERS], writer;
    int reader_ids[NUM_READERS];
    
    // Start readers
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, test2_reader, &reader_ids[i]);
        usleep(50000); // Small delay between reader starts
    }
    
    // Wait 5 seconds, then start writer
    sleep(5);
    pthread_create(&writer, NULL, test2_writer, NULL);
    
    // Wait for threads to finish
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    pthread_join(writer, NULL);
    
    // Calculate earliest reader end time
    time_t earliest_reader_end = readers_end_times[0];
    for (int i = 1; i < NUM_READERS; i++) {
        if (readers_end_times[i] < earliest_reader_end) {
            earliest_reader_end = readers_end_times[i];
        }
    }
    
    // Check results
    printf("\nTest 2 Results:\n");
    if (writer_accessed_during_read) {
        printf("TEST FAILED: Writer accessed resource while readers were active\n");
    } else if (writer_start_time < earliest_reader_end) {
        printf("TEST FAILED: Writer acquired lock before all readers finished\n");
    } else {
        printf("TEST PASSED: No context switch occurred before readers finished\n");
    }
    
    printf("Average reader duration: %.2f seconds\n", calculate_avg_duration(readers_start_times, readers_end_times, NUM_READERS));
    printf("Writer waited approximately: %ld seconds\n", 
           writer_start_time - (readers_start_times[0] + 5));
}

int main() {
    // Run Test 1
    run_test1();
    
    // Add a delay between tests
    sleep(2);
    
    // Run Test 2
    run_test2();
    
    return 0;
}