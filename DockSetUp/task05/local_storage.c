#include "local_storage.h"
#include <stdio.h>
#include <stdbool.h>
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
    int64_t current_id = (int64_t)pthread_self(); // current id
    int open_position = -1; // tracks empty slots

    for (int i = 0; i < MAX_THREADS; i++)
    {
        // checks if the thread exists and is the current thread
        if (search(g_tls[i].thread_id) || g_tls[i].thread_id == current_id)
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

    //checks if an empty postion was found
    if (open_position == -1)
    {
        printf("thread [%ld] failed to initialize, not enough space\n", current_id);
        exit(1)
    }
    else
    {
        // allocates the thread id at the first open position
        g_tls[open_position].thread_id = current_id;
    }
    /// TSVI Comments
    // check if the thread exists and is the current thread

    //-1 says its free/empty to use
    // find an empty/free thread and assign it to me

    // if full print
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement get_tls_data to retrieve the TLS data for the calling thread.
 */
void *get_tls_data(void)
{
    // TODO: Search for the calling thread's entry and return its data.
    return NULL;
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement set_tls_data to update the TLS data for the calling thread.
 */
void set_tls_data(void *data)
{
    // TODO: Search for the calling thread's entry and set its data.
}
//--------------------------------------------------------------------//
/*
 * TODO: Implement tls_thread_free to free the TLS entry for the calling thread.
 */
void tls_thread_free(void)
{
    // TODO: Reset the thread_id and data in the corresponding TLS entry.
}
//--------------------------------------------------------------------//
/*
 * searches the array and returns true if the current thread is found
 */
bool search(int target)
{
    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (g_tls[i].thread_id == target)
        {
            return true;
        }
    }
    return false;
}
//----------------------------------End of File----------------------------------//
