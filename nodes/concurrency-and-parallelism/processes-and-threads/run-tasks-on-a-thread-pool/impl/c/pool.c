// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

#include "pool.h"

#include <pthread.h>

// The shared state of the pool. The queue is the range of indexes from next to n.
struct pool {
    const int32_t *numbers;
    int32_t *out;
    size_t n;
    size_t next;  // guarded by lock
    pthread_mutex_t lock;
};

static int32_t collatz_steps(int64_t n) {
    int32_t steps = 0;
    while (n != 1) {
        n = n % 2 == 0 ? n / 2 : 3 * n + 1;
        steps++;
    }
    return steps;
}

static void *worker(void *arg) {
    struct pool *p = arg;
    for (;;) {
        // Take the next job under the lock, then run it without the lock.
        pthread_mutex_lock(&p->lock);
        size_t i = p->next;
        if (i < p->n) {
            p->next++;
        }
        pthread_mutex_unlock(&p->lock);
        if (i >= p->n) {
            return NULL;
        }
        p->out[i] = collatz_steps(p->numbers[i]);  // each job writes its own element
    }
}

bool steps_on_pool(size_t workers, const int32_t numbers[], size_t n, int32_t out[]) {
    if (workers == 0 || workers > POOL_MAX_WORKERS) {
        return false;
    }
    struct pool p = {.numbers = numbers, .out = out, .n = n, .next = 0};
    if (pthread_mutex_init(&p.lock, NULL) != 0) {
        return false;
    }
    pthread_t threads[POOL_MAX_WORKERS];
    size_t started = 0;
    while (started < workers && pthread_create(&threads[started], NULL, worker, &p) == 0) {
        started++;
    }
    // The threads that did start take all the jobs between them.
    for (size_t i = 0; i < started; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&p.lock);
    return started > 0;
}
