// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

#ifndef TLS_H
#define TLS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TLS_MAX_THREADS 16

// Adds 1 to the counter of the current thread and returns the new value.
int32_t tls_bump(void);

// Returns the counter of the current thread.
int32_t tls_current(void);

// Starts one thread for each of the n counts. Thread i calls tls_bump counts[i] times and writes
// its final counter to out[i]. Returns false if n is more than TLS_MAX_THREADS or a thread does not start.
bool count_per_thread(const int32_t counts[], size_t n, int32_t out[]);

#endif
