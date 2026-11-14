// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

#include "remove_duplicate_lines.h"

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// read_all reads the whole stream into a new buffer. The buffer has at least
// one free byte after the data, for a NUL.
static char *read_all(FILE *f, size_t *size) {
    size_t cap = 4096;
    size_t used = 0;
    char *data = malloc(cap);
    if (data == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    for (;;) {
        if (cap - used < 2) {
            char *bigger = cap <= SIZE_MAX / 2 ? realloc(data, cap * 2) : NULL;
            if (bigger == NULL) {
                free(data);
                errno = ENOMEM;
                return NULL;
            }
            data = bigger;
            cap *= 2;
        }
        size_t n = fread(data + used, 1, cap - used - 1, f);
        used += n;
        if (n == 0) {
            break;
        }
    }
    if (ferror(f)) {
        free(data);
        errno = EIO;
        return NULL;
    }
    *size = used;
    return data;
}

// hash is the 64-bit FNV-1a hash of the bytes of a line.
static uint64_t hash(const char *s, size_t n) {
    uint64_t h = 14695981039346656037u;
    for (size_t i = 0; i < n; i++) {
        h = (h ^ (unsigned char)s[i]) * 1099511628211u;
    }
    return h;
}

// A set of lines: an open addressing table with linear probing. Each slot
// holds 0 for an empty slot, or 1 plus the index of a line in lines.
struct set {
    size_t *slots;
    size_t cap;  // a power of 2
};

static bool same(const struct line *a, const struct line *b) {
    return a->len == b->len && memcmp(a->text, b->text, a->len) == 0;
}

// find returns the slot of the line, or the empty slot where it belongs.
static size_t find(const struct set *s, const struct line *lines, const struct line *l) {
    size_t i = (size_t)(hash(l->text, l->len) & (s->cap - 1));
    while (s->slots[i] != 0 && !same(&lines[s->slots[i] - 1], l)) {
        i = (i + 1) & (s->cap - 1);
    }
    return i;
}

// grow doubles the table and puts the n lines into it again.
static int grow(struct set *s, const struct line *lines, size_t n) {
    struct set bigger = {calloc(s->cap * 2, sizeof(size_t)), s->cap * 2};
    if (bigger.slots == NULL) {
        return -1;
    }
    for (size_t k = 0; k < n; k++) {
        bigger.slots[find(&bigger, lines, &lines[k])] = k + 1;
    }
    free(s->slots);
    *s = bigger;
    return 0;
}

int remove_duplicate_lines(const char *path, struct unique_lines *out) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    size_t size;
    char *data = read_all(f, &size);
    fclose(f);
    if (data == NULL) {
        return -1;
    }
    // Each LF ends a line. A last line with no LF is one more line.
    size_t count = 0;
    for (size_t i = 0; i < size; i++) {
        count += data[i] == '\n';
    }
    if (size > 0 && data[size - 1] != '\n') {
        count++;
    }
    struct line *lines = count > 0 ? malloc(count * sizeof *lines) : NULL;
    struct set set = {calloc(16, sizeof(size_t)), 16};
    if ((count > 0 && lines == NULL) || set.slots == NULL) {
        free(lines);
        free(set.slots);
        free(data);
        errno = ENOMEM;
        return -1;
    }
    // Change each LF to a NUL, so that each line is a C string too. The end
    // of the file ends a last line with no LF. Keep a line only if the set
    // does not have it yet.
    size_t n = 0;
    size_t start = 0;
    for (size_t i = 0; i <= size; i++) {
        bool end = i < size ? data[i] == '\n' : start < size;
        if (!end) {
            continue;
        }
        data[i] = '\0';
        struct line l = {data + start, i - start};
        start = i + 1;
        size_t slot = find(&set, lines, &l);
        if (set.slots[slot] != 0) {
            continue;
        }
        lines[n++] = l;
        set.slots[slot] = n;
        // Keep the table less than half full, so that a probe ends soon.
        if (2 * n > set.cap && grow(&set, lines, n) != 0) {
            free(lines);
            free(set.slots);
            free(data);
            errno = ENOMEM;
            return -1;
        }
    }
    free(set.slots);
    *out = (struct unique_lines){lines, n, data};
    return 0;
}

void unique_lines_free(struct unique_lines *u) {
    free(u->lines);
    free(u->data);
    *u = (struct unique_lines){NULL, 0, NULL};
}
