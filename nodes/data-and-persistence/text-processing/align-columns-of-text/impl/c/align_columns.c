// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

#include "align_columns.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

enum align { LEFT, RIGHT, CENTER };

// A field is n bytes at s, in the text.
struct field {
    const char *s;
    size_t n;
    size_t width;  // the number of code points
};

// Returns the number of code points in the n bytes at s. A UTF-8 byte from
// 0x80 to 0xBF continues a code point, so the code counts the other bytes.
static size_t code_points(const char *s, size_t n) {
    size_t count = 0;
    for (size_t i = 0; i < n; i++) {
        count += ((unsigned char)s[i] & 0xC0) != 0x80;
    }
    return count;
}

// Adds an item to the array *items with *len items, and makes the array
// larger if it is full. Returns 0, or -1 if there is no memory.
static int push(void **items, size_t *len, size_t *cap, size_t size, const void *item) {
    if (*len == *cap) {
        size_t bigger = *cap == 0 ? 16 : *cap * 2;
        void *p = realloc(*items, bigger * size);
        if (p == NULL) {
            return -1;
        }
        *items = p;
        *cap = bigger;
    }
    memcpy((char *)*items + *len * size, item, size);
    (*len)++;
    return 0;
}

char *align_columns(const char *text, const char *align) {
    enum align a;
    if (strcmp(align, "left") == 0) {
        a = LEFT;
    } else if (strcmp(align, "right") == 0) {
        a = RIGHT;
    } else if (strcmp(align, "center") == 0) {
        a = CENTER;
    } else {
        errno = EINVAL;
        return NULL;
    }
    struct field *fields = NULL;  // the fields of all the lines, in order
    size_t nfields = 0, fields_cap = 0;
    size_t *counts = NULL;  // the number of fields in each line
    size_t nlines = 0, counts_cap = 0;
    size_t *widths = NULL;
    size_t columns = 0;
    char *out = NULL;

    // Split the text into lines and fields.
    for (const char *s = text; *s != '\0';) {
        const char *lf = strchr(s, '\n');
        const char *end = lf != NULL ? lf : s + strlen(s);
        size_t count = 0;
        // A $ at the end of the line does not start a new field, and an
        // empty line has no fields.
        for (const char *p = s; p < end; count++) {
            const char *dollar = memchr(p, '$', (size_t)(end - p));
            const char *stop = dollar != NULL ? dollar : end;
            struct field f = {p, (size_t)(stop - p), code_points(p, (size_t)(stop - p))};
            if (push((void **)&fields, &nfields, &fields_cap, sizeof f, &f) != 0) {
                goto no_memory;
            }
            p = dollar != NULL ? dollar + 1 : end;
        }
        if (push((void **)&counts, &nlines, &counts_cap, sizeof count, &count) != 0) {
            goto no_memory;
        }
        if (count > columns) {
            columns = count;
        }
        s = lf != NULL ? lf + 1 : end;
    }

    // Find the width of each column.
    widths = calloc(columns + 1, sizeof *widths);
    if (widths == NULL) {
        goto no_memory;
    }
    for (size_t i = 0, k = 0; i < nlines; i++) {
        for (size_t j = 0; j < counts[i]; j++, k++) {
            if (fields[k].width > widths[j]) {
                widths[j] = fields[k].width;
            }
        }
    }

    // The first pass finds the size of the result, and the second writes it.
    size_t size = 0;
    for (int pass = 0; pass < 2; pass++) {
        size_t at = 0;
        for (size_t i = 0, k = 0; i < nlines; i++) {
            for (size_t j = 0; j < counts[i]; j++, k++) {
                size_t pad = widths[j] - fields[k].width;
                size_t left = a == LEFT ? 0 : a == RIGHT ? pad : pad / 2;
                size_t right = j + 1 < counts[i] ? pad - left : 0;
                if (pass == 1) {
                    char *p = out + at;
                    if (j > 0) {
                        *p++ = ' ';
                    }
                    memset(p, ' ', left);
                    memcpy(p + left, fields[k].s, fields[k].n);
                    memset(p + left + fields[k].n, ' ', right);
                }
                at += (j > 0) + left + fields[k].n + right;
            }
            if (pass == 1) {
                out[at] = '\n';
            }
            at++;
        }
        if (pass == 0) {
            size = at;
            out = malloc(size + 1);
            if (out == NULL) {
                goto no_memory;
            }
        }
    }
    out[size] = '\0';
    free(fields);
    free(counts);
    free(widths);
    return out;

no_memory:
    free(fields);
    free(counts);
    free(widths);
    errno = ENOMEM;
    return NULL;
}
