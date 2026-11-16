// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

// Count on a thread-local counter in one thread for each count, with no lock and no shared counter.

#include "tls.h"

#include <pthread.h>

// Each thread has its own copy of this variable, and each copy starts at 0.
// C23 also accepts the spelling thread_local.
static _Thread_local int32_t counter = 0;

int32_t tls_bump(void) {
    return ++counter;
}

int32_t tls_current(void) {
    return counter;
}

struct job {
    int32_t count;
    int32_t result;
};

static void *work(void *arg) {
    struct job *job = arg;
    for (int32_t k = 0; k < job->count; k++) {
        tls_bump();
    }
    // Each thread writes only its own job. pthread_join makes the write visible to the caller.
    job->result = tls_current();
    return NULL;
}

bool count_per_thread(const int32_t counts[], size_t n, int32_t out[]) {
    if (n > TLS_MAX_THREADS) {
        return false;
    }
    pthread_t threads[TLS_MAX_THREADS];
    struct job jobs[TLS_MAX_THREADS];
    size_t started = 0;
    bool ok = true;
    for (; started < n; started++) {
        jobs[started] = (struct job){.count = counts[started], .result = 0};
        if (pthread_create(&threads[started], NULL, work, &jobs[started]) != 0) {
            ok = false;
            break;
        }
    }
    // Join every thread that started, also after an error.
    for (size_t i = 0; i < started; i++) {
        pthread_join(threads[i], NULL);
    }
    if (!ok) {
        return false;
    }
    for (size_t i = 0; i < n; i++) {
        out[i] = jobs[i].result;
    }
    return true;
}
