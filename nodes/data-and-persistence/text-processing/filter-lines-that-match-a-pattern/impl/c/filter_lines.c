// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

#include "filter_lines.h"

#include <errno.h>
#include <regex.h>
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

int filter_lines(const char *path, const char *pattern, struct matched_lines *out) {
    // REG_EXTENDED selects the extended syntax, as grep -E does. REG_NOSUB
    // asks regexec only if the line matches, not where.
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) != 0) {
        errno = EINVAL;
        return -1;
    }
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        int e = errno;
        regfree(&re);
        errno = e;
        return -1;
    }
    size_t size;
    char *data = read_all(f, &size);
    int e = errno;
    fclose(f);
    if (data == NULL) {
        regfree(&re);
        errno = e;
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
    if (count > 0 && lines == NULL) {
        regfree(&re);
        free(data);
        errno = ENOMEM;
        return -1;
    }
    // Change each LF to a NUL, so that each line is a C string, which is what
    // regexec takes. The end of the file ends a last line with no LF.
    size_t n = 0;
    size_t start = 0;
    for (size_t i = 0; i <= size; i++) {
        if (i < size ? data[i] != '\n' : start == size) {
            continue;
        }
        data[i] = '\0';
        if (regexec(&re, data + start, 0, NULL, 0) == 0) {
            lines[n++] = (struct line){data + start, i - start};
        }
        start = i + 1;
    }
    regfree(&re);
    *out = (struct matched_lines){lines, n, data};
    return 0;
}

void matched_lines_free(struct matched_lines *m) {
    free(m->lines);
    free(m->data);
    *m = (struct matched_lines){NULL, 0, NULL};
}
