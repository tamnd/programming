// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

#include "greetings.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct job {
    const char *name;
    char *greeting;  // written by the thread
};

static void *greet(void *arg) {
    struct job *job = arg;
    const char *prefix = "hello from ";
    size_t size = strlen(prefix) + strlen(job->name) + 1;
    job->greeting = malloc(size);
    if (job->greeting != NULL) {
        snprintf(job->greeting, size, "%s%s", prefix, job->name);
    }
    return NULL;
}

char **thread_greetings(const char *const names[], size_t n) {
    struct job *jobs = calloc(n > 0 ? n : 1, sizeof *jobs);
    pthread_t *threads = calloc(n > 0 ? n : 1, sizeof *threads);
    char **out = calloc(n > 0 ? n : 1, sizeof *out);
    if (jobs == NULL || threads == NULL || out == NULL) {
        free(jobs);
        free(threads);
        free(out);
        return NULL;
    }
    size_t started = 0;
    for (; started < n; started++) {
        jobs[started].name = names[started];
        if (pthread_create(&threads[started], NULL, greet, &jobs[started]) != 0) {
            break;
        }
    }
    // Join every thread that started, even after a failure, so that none is left running.
    bool ok = started == n;
    for (size_t i = 0; i < started; i++) {
        pthread_join(threads[i], NULL);
        out[i] = jobs[i].greeting;
        ok = ok && out[i] != NULL;
    }
    free(jobs);
    free(threads);
    if (!ok) {
        free_greetings(out, started);
        return NULL;
    }
    return out;
}

void free_greetings(char **greetings, size_t n) {
    if (greetings == NULL) {
        return;
    }
    for (size_t i = 0; i < n; i++) {
        free(greetings[i]);
    }
    free(greetings);
}
