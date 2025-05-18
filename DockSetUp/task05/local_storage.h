#ifndef LOCAL_STORAGE_H
#define LOCAL_STORAGE_H

#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_THREADS 100

/*
 * Structure to hold thread-specific arbitrary data.
 */
typedef struct {
    int64_t thread_id;  // For an unused entry, initialize to -1
    void* data;         // Initialize to NULL
} tls_data_t;
//--------------------------------------------------------------------//
/*
 * Global TLS array.
 * Students should define this array in local_storage.c.
 */
extern tls_data_t g_tls[MAX_THREADS];
//--------------------------------------------------------------------//
/*
 * Initializes g_tls as an array of MAX_THREADS entries.
 */
void init_storage(void);
//--------------------------------------------------------------------//
/*
 * Initializes the TLS entry for the calling thread.
 */
void tls_thread_alloc(void);
//--------------------------------------------------------------------//
/*
 * Returns a pointer to the arbitrary data stored for the calling thread.
 */
void* get_tls_data(void);
//--------------------------------------------------------------------//
/*
 * Sets the given void* data to the TLS for the calling thread.
 */
void set_tls_data(void* data);
//--------------------------------------------------------------------//
/*
 * Frees the TLS entry for the calling thread.
 */
void tls_thread_free(void);
//--------------------------------------------------------------------//
/*
 * searches the array and returns true if the current thread is found
 
bool search(int target)
*/

#endif // LOCAL_STORAGE_H
//----------------------------------End of File----------------------------------//
