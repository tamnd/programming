// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

#include "first_and_last_lines.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

int first_and_last_lines(const char *path, size_t count, struct first_and_last *out) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    size_t size;
    char *data = read_all(f, &size);
    int e = errno;
    fclose(f);
    if (data == NULL) {
        errno = e;
        return -1;
    }
    // Each LF ends a line. A last line with no LF is one more line.
    size_t n = 0;
    for (size_t i = 0; i < size; i++) {
        n += data[i] == '\n';
    }
    if (size > 0 && data[size - 1] != '\n') {
        n++;
    }
    struct line *lines = n > 0 ? malloc(n * sizeof *lines) : NULL;
    if (n > 0 && lines == NULL) {
        free(data);
        errno = ENOMEM;
        return -1;
    }
    // Change each LF to a NUL, so that each line is a C string. The end of the
    // file ends a last line with no LF.
    size_t k = 0;
    size_t start = 0;
    for (size_t i = 0; i <= size; i++) {
        if (i < size ? data[i] != '\n' : start == size) {
            continue;
        }
        data[i] = '\0';
        lines[k++] = (struct line){data + start, i - start};
        start = i + 1;
    }
    // The first part starts at line 0, and the last part ends at line n - 1. An
    // empty file has no array of lines, and lines is NULL.
    size_t take = count < n ? count : n;
    const struct line *last = lines == NULL ? NULL : lines + (n - take);
    *out = (struct first_and_last){lines, take, last, take, lines, data};
    return 0;
}

void first_and_last_free(struct first_and_last *r) {
    free(r->lines);
    free(r->data);
    *r = (struct first_and_last){NULL, 0, NULL, 0, NULL, NULL};
}
