// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

#include "round_robin.h"

#include <stdlib.h>

bool finish_order(const int32_t *work, size_t n, int32_t slice, size_t *order) {
    if (n == 0) {
        return true;
    }
    // C has no queue type. The queue is a ring buffer of n slots: the queue
    // never holds more than n jobs. head is the front, and count is the
    // number of jobs in the queue.
    size_t *queue = malloc(n * sizeof *queue);
    int32_t *left = malloc(n * sizeof *left);
    if (queue == NULL || left == NULL) {
        free(queue);
        free(left);
        return false;
    }
    for (size_t i = 0; i < n; i++) {
        queue[i] = i;
        left[i] = work[i];
    }
    size_t head = 0;
    size_t count = n;
    size_t done = 0;
    while (count > 0) {
        size_t job = queue[head]; // dequeue
        head = (head + 1) % n;
        count--;
        left[job] -= left[job] < slice ? left[job] : slice;
        if (left[job] == 0) {
            order[done++] = job;
        } else {
            queue[(head + count) % n] = job; // enqueue at the back
            count++;
        }
    }
    free(queue);
    free(left);
    return true;
}
