// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

#include "serve_order.h"

#include <stdlib.h>

// C has no priority queue type. This is a binary heap of request numbers in
// an array: the children of slot i are the slots 2i+1 and 2i+2.
struct heap {
    const int32_t *priority;
    size_t *items;
    size_t count;
};

// before is true if request a comes out before request b. It compares the
// priorities with < and ==, not with a subtraction that can overflow.
static bool before(const struct heap *h, size_t a, size_t b) {
    if (h->priority[a] != h->priority[b]) {
        return h->priority[a] > h->priority[b];
    }
    return a < b;
}

static void swap(size_t *x, size_t *y) {
    size_t t = *x;
    *x = *y;
    *y = t;
}

static void push(struct heap *h, size_t request) {
    size_t i = h->count++;
    h->items[i] = request;
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (!before(h, h->items[i], h->items[parent])) {
            break;
        }
        swap(&h->items[i], &h->items[parent]);
        i = parent;
    }
}

static size_t pop(struct heap *h) {
    size_t top = h->items[0];
    h->items[0] = h->items[--h->count];
    size_t i = 0;
    for (;;) {
        size_t l = 2 * i + 1;
        size_t r = l + 1;
        size_t best = i;
        if (l < h->count && before(h, h->items[l], h->items[best])) {
            best = l;
        }
        if (r < h->count && before(h, h->items[r], h->items[best])) {
            best = r;
        }
        if (best == i) {
            return top;
        }
        swap(&h->items[i], &h->items[best]);
        i = best;
    }
}

bool serve_order(const int32_t *priorities, size_t n, size_t *order) {
    if (n == 0) {
        return true;
    }
    struct heap h = {priorities, malloc(n * sizeof(size_t)), 0};
    if (h.items == NULL) {
        return false;
    }
    for (size_t i = 0; i < n; i++) {
        push(&h, i);
    }
    for (size_t i = 0; i < n; i++) {
        order[i] = pop(&h);
    }
    free(h.items);
    return true;
}
