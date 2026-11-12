// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

#include "number_line.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A buffer that keeps its length, so an append does not scan for the end,
// and that doubles its capacity when it is full.
struct buf {
    char *data;
    size_t len;
    size_t cap;
};

static int append(struct buf *b, const char *s, size_t n) {
    if (b->len + n + 1 > b->cap) {
        size_t cap = b->cap ? b->cap : 16;
        while (b->len + n + 1 > cap) {
            cap *= 2;
        }
        char *p = realloc(b->data, cap);
        if (p == NULL) {
            return -1;
        }
        b->data = p;
        b->cap = cap;
    }
    memcpy(b->data + b->len, s, n);
    b->len += n;
    b->data[b->len] = '\0';
    return 0;
}

char *number_line(int n) {
    struct buf b = {NULL, 0, 0};
    if (append(&b, "", 0) != 0) {
        return NULL;
    }
    char digits[16];
    for (int i = 1; i <= n; i++) {
        int k = snprintf(digits, sizeof digits, i > 1 ? ",%d" : "%d", i);
        if (append(&b, digits, (size_t)k) != 0) {
            free(b.data);
            return NULL;
        }
    }
    return b.data;
}
