// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

#include "tls.h"

#include <pthread.h>
#include <stdio.h>

static int failures = 0;

static void check(const char *name, const int32_t counts[], size_t n) {
    int32_t got[TLS_MAX_THREADS] = {0};
    if (!count_per_thread(counts, n, got)) {
        fprintf(stderr, "case %s failed: the threads did not start\n", name);
        failures++;
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (got[i] != counts[i]) {
            fprintf(stderr, "case %s failed at %zu: got %d, want %d\n", name, i, got[i], counts[i]);
            failures++;
            return;
        }
    }
}

static void *caller(void *arg) {
    int32_t *seen = arg;
    for (int k = 0; k < 3; k++) {
        tls_bump();
    }
    int32_t out[2];
    count_per_thread((int32_t[]){5, 6}, 2, out);
    *seen = tls_current();
    return NULL;
}

int main(void) {
    check("three-threads", (int32_t[]){1, 2, 3}, 3);
    check("uneven", (int32_t[]){10, 0, 5, 1000}, 4);
    check("empty", NULL, 0);
    check("zero", (int32_t[]){0}, 1);
    check("one-thread", (int32_t[]){7}, 1);
    check("equal-counts", (int32_t[]){100, 100, 100, 100}, 4);
    int32_t many[16];
    for (int i = 0; i < 16; i++) {
        many[i] = i + 1;
    }
    check("many-threads", many, 16);
    check("large", (int32_t[]){100000, 100000}, 2);

    // Run in a new thread, so that the counter of the main thread does not matter.
    int32_t seen = -1;
    pthread_t t;
    if (pthread_create(&t, NULL, caller, &seen) != 0 || pthread_join(t, NULL) != 0 || seen != 3) {
        fprintf(stderr, "case caller-counter failed: got %d, want 3\n", seen);
        failures++;
    }
    int32_t out[TLS_MAX_THREADS + 1];
    int32_t too_many[TLS_MAX_THREADS + 1] = {0};
    if (count_per_thread(too_many, TLS_MAX_THREADS + 1, out)) {
        fprintf(stderr, "case too-many failed: want false\n");
        failures++;
    }
    if (failures == 0) {
        printf("all tests passed\n");
    }
    return failures == 0 ? 0 : 1;
}
