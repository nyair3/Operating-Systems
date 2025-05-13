#include "local_storage.h"

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

/*
 * TODO: Implement tls_thread_alloc to allocate a TLS entry for the calling thread.
 */
void tls_thread_alloc(void)
{
    // TODO: Use your synchronization mechanism to safely allocate an entry.
    int64_t current_id = (int64_t)pthread_self();
    int index = -1;

    for (int i = 0; i< MAX_THREADS; i++)
    {
        if (g_tls[i].thread_id == -1)
        {
            g_tls[i].thread_id = current_id;
        }
        else
        {
            return;
        }
    }
    if (index == -1)
    {
        printf("thread [%ld] failed to initialize, not enough space\n", g_tls[i].thread_id);
        exit(1);
    }
}

/*
 * TODO: Implement get_tls_data to retrieve the TLS data for the calling thread.
 */
void *get_tls_data(void)
{
    // TODO: Search for the calling thread's entry and return its data.
    return NULL;
}

/*
 * TODO: Implement set_tls_data to update the TLS data for the calling thread.
 */
void set_tls_data(void *data)
{
    // TODO: Search for the calling thread's entry and set its data.
}

/*
 * TODO: Implement tls_thread_free to free the TLS entry for the calling thread.
 */
void tls_thread_free(void)
{
    // TODO: Reset the thread_id and data in the corresponding TLS entry.
}
