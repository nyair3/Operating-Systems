#include "local_storage.h"
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
/*
 * TODO: Define the global TLS array.
 */
tls_data_t g_tls[MAX_THREADS];

/*
 * TODO: Implement init_storage to initialize g_tls.
 */
void init_storage(void)
{
    // TODO: Set all thread_id fields to -1 and data pointers to NULL.
    for (int i = 0; i < MAX_THREADS; i++)
    {
        g_tls[i].thread_id = -1;
        g_tls[i].data = NULL;
    }
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement tls_thread_alloc to allocate a TLS entry for the calling thread.
 */
void tls_thread_alloc(void)
{
    // TODO: Use your synchronization mechanism to safely allocate an entry.
    int64_t current_id = (int64_t)pthread_self();
    int open_position = -1;

    for (int i = 0; i < MAX_THREADS; i++)
    {
        // checks if the thread exists and is the current thread
        if (g_tls[i].thread_id == current_id)
        {
            // Thread already initialized
            printf("Already initialized");
            return;
        }
    }
    // Finds the first empty slot
    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (g_tls[i].thread_id == -1)
        {
            open_position = i;
            break;
        }
    }

    // checks if an empty postion can be found
    if (open_position == -1)
    {
        printf("thread [%ld] failed to initialize, not enough space\n", current_id);
        exit(1);
    }
    else
    {
        // allocates the thread id at the first empty/available position
        g_tls[open_position].thread_id = current_id;
    }
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement get_tls_data to retrieve the TLS data for the calling thread.
 */
void *get_tls_data(void)
{
    // TODO: Search for the calling thread's entry and return its data.
    int64_t current_id = (int64_t)pthread_self();

    // Finds the entry for the cuerrnt thread thread
    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (g_tls[i].thread_id == current_id)
        {
            return g_tls[i].data;
        }
    }

    // Prints if the entry for this thread is not found
    printf("thread [%ld] has not been initialized in TLS\n", current_id);
    exit(2);
    return NULL;
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement set_tls_data to update the TLS data for the calling thread.
 */
void set_tls_data(void *data)
{
    // TODO: Search for the calling thread's entry and set its data.
    int64_t current_id = (int64_t)pthread_self();

    // Finds the entry for the cuerrnt thread and updates the TLS data for the calling thread
    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (g_tls[i].thread_id == current_id)
        {
            g_tls[i].data = data;
            return;
        }
    }

    // Prints if the entry for this thread is not found
    printf("thread [%ld] has not been initialized in TLS\n", current_id);
    exit(2);
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement tls_thread_free to free the TLS entry for the calling thread.
 */
void tls_thread_free(void)
{
    // TODO: Reset the thread_id and data in the corresponding TLS entry.
    int64_t current_id = (int64_t)pthread_self();

    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (g_tls[i].thread_id == current_id)
        {
            // Resets the thread_id and data in the corresponding TLS entry
            g_tls[i].thread_id = -1;
            g_tls[i].data = NULL;
            return;
        }
    }

    // Prints if the entry for this thread is not found
    printf("thread [%ld] has not been initialized in TLS\n", current_id);
    exit(2);
}
//----------------------------------End of File----------------------------------//
