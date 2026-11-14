// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

#include "sort_lines.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// compare orders two lines by their bytes, as unsigned char. If one line is
// the start of the other, the shorter line comes first.
static int compare(const void *a, const void *b) {
    const struct line *x = a;
    const struct line *y = b;
    int c = memcmp(x->text, y->text, x->len < y->len ? x->len : y->len);
    if (c != 0) {
        return c;
    }
    return (x->len > y->len) - (x->len < y->len);
}

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

int sort_lines(const char *path, struct sorted_lines *out) {
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
    struct line *lines = NULL;
    if (count > 0) {
        lines = malloc(count * sizeof *lines);
        if (lines == NULL) {
            free(data);
            errno = ENOMEM;
            return -1;
        }
    }
    // Change each LF to a NUL, so that each line is a C string too.
    size_t k = 0;
    size_t start = 0;
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\n') {
            data[i] = '\0';
            lines[k++] = (struct line){data + start, i - start};
            start = i + 1;
        }
    }
    if (start < size) {
        data[size] = '\0';
        lines[k++] = (struct line){data + start, size - start};
    }
    if (count > 1) {
        qsort(lines, count, sizeof *lines, compare);
    }
    *out = (struct sorted_lines){lines, count, data};
    return 0;
}

void sort_lines_free(struct sorted_lines *s) {
    free(s->lines);
    free(s->data);
    *s = (struct sorted_lines){NULL, 0, NULL};
}
