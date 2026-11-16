// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

#include "sums.h"

#include <pthread.h>
#include <stdlib.h>

struct job {
    const long long *items;
    size_t length;
    long long sum;
};

static void *sum_chunk(void *arg) {
    struct job *job = arg;
    job->sum = 0;
    for (size_t i = 0; i < job->length; i++) {
        job->sum += job->items[i];
    }
    // pthread_join hands this pointer to the caller.
    return &job->sum;
}

bool parallel_sums(const long long *const chunks[], const size_t lengths[], size_t n, long long out[]) {
    struct job *jobs = calloc(n > 0 ? n : 1, sizeof *jobs);
    pthread_t *threads = calloc(n > 0 ? n : 1, sizeof *threads);
    if (jobs == NULL || threads == NULL) {
        free(jobs);
        free(threads);
        return false;
    }
    size_t started = 0;
    for (; started < n; started++) {
        jobs[started] = (struct job){chunks[started], lengths[started], 0};
        if (pthread_create(&threads[started], NULL, sum_chunk, &jobs[started]) != 0) {
            break;
        }
    }
    // Join every thread that started, even after a failure, before the jobs are freed.
    for (size_t i = 0; i < started; i++) {
        void *result = NULL;
        pthread_join(threads[i], &result);
        out[i] = *(long long *)result;
    }
    free(jobs);
    free(threads);
    return started == n;
}
