// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

#include "diff_two_texts.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct line {
    const char *text;  // the bytes of the line, in the text; not ended by a NUL
    size_t len;        // the number of bytes, with no LF
};

// split_lines splits text at each LF into a new array, and sets *count. A
// final LF does not start a new line. It returns NULL for an empty text too,
// with *count 0, so the caller checks errno only if *count is not 0.
static struct line *split_lines(const char *text, size_t *count) {
    size_t n = 0;
    for (const char *p = text; *p != '\0'; n++) {
        const char *lf = strchr(p, '\n');
        p = lf != NULL ? lf + 1 : p + strlen(p);
    }
    *count = n;
    if (n == 0) {
        return NULL;
    }
    struct line *lines = malloc(n * sizeof *lines);
    if (lines == NULL) {
        return NULL;
    }
    size_t i = 0;
    for (const char *p = text; *p != '\0'; i++) {
        const char *lf = strchr(p, '\n');
        size_t len = lf != NULL ? (size_t)(lf - p) : strlen(p);
        lines[i] = (struct line){p, len};
        p += lf != NULL ? len + 1 : len;
    }
    return lines;
}

static int equal(struct line x, struct line y) {
    return x.len == y.len && memcmp(x.text, y.text, x.len) == 0;
}

// put writes the mark, the line and an LF at *out, and moves *out after them.
static void put(char **out, char mark, struct line l) {
    *(*out)++ = mark;
    memcpy(*out, l.text, l.len);
    *out += l.len;
    *(*out)++ = '\n';
}

char *diff_lines(const char *old_text, const char *new_text) {
    size_t n, m;
    struct line *a = split_lines(old_text, &n);
    if (a == NULL && n > 0) {
        errno = ENOMEM;
        return NULL;
    }
    struct line *b = split_lines(new_text, &m);
    if (b == NULL && m > 0) {
        free(a);
        errno = ENOMEM;
        return NULL;
    }
    size_t w = m + 1;
    // lcs[i * w + j] is the length of a longest common subsequence of the
    // lines from a[i] and from b[j]. Row n and column m stay 0.
    size_t *lcs = NULL;
    if (n + 1 <= SIZE_MAX / sizeof *lcs / w) {
        lcs = calloc((n + 1) * w, sizeof *lcs);
    }
    // Each line of the script is one line of a or b, with a mark and an LF.
    char *result = malloc(strlen(old_text) + strlen(new_text) + 2 * (n + m) + 1);
    if (lcs == NULL || result == NULL) {
        free(a);
        free(b);
        free(lcs);
        free(result);
        errno = ENOMEM;
        return NULL;
    }
    for (size_t i = n; i-- > 0;) {
        for (size_t j = m; j-- > 0;) {
            if (equal(a[i], b[j])) {
                lcs[i * w + j] = lcs[(i + 1) * w + j + 1] + 1;
            } else {
                size_t down = lcs[(i + 1) * w + j], right = lcs[i * w + j + 1];
                lcs[i * w + j] = down >= right ? down : right;
            }
        }
    }
    char *out = result;
    size_t i = 0, j = 0;
    while (i < n && j < m) {
        if (equal(a[i], b[j])) {
            put(&out, ' ', a[i]);
            i++;
            j++;
        } else if (lcs[(i + 1) * w + j] >= lcs[i * w + j + 1]) {
            put(&out, '-', a[i++]);
        } else {
            put(&out, '+', b[j++]);
        }
    }
    while (i < n) {
        put(&out, '-', a[i++]);
    }
    while (j < m) {
        put(&out, '+', b[j++]);
    }
    *out = '\0';
    free(a);
    free(b);
    free(lcs);
    return result;
}

// read_file reads the whole file at path into a new string that ends with a
// NUL. On an error it returns NULL and sets errno.
static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return NULL;
    }
    size_t cap = 4096, used = 0;
    char *data = malloc(cap);
    for (;;) {
        if (data == NULL) {
            fclose(f);
            errno = ENOMEM;
            return NULL;
        }
        used += fread(data + used, 1, cap - used - 1, f);
        if (used < cap - 1) {
            break;
        }
        char *bigger = cap <= SIZE_MAX / 2 ? realloc(data, cap * 2) : NULL;
        if (bigger == NULL) {
            free(data);
        }
        data = bigger;
        cap *= 2;
    }
    int failed = ferror(f);
    fclose(f);
    if (failed) {
        free(data);
        errno = EIO;
        return NULL;
    }
    data[used] = '\0';
    return data;
}

char *diff_files(const char *old_path, const char *new_path) {
    char *old_text = read_file(old_path);
    if (old_text == NULL) {
        return NULL;
    }
    char *new_text = read_file(new_path);
    if (new_text == NULL) {
        int saved = errno;
        free(old_text);
        errno = saved;
        return NULL;
    }
    char *result = diff_lines(old_text, new_text);
    int saved = errno;
    free(old_text);
    free(new_text);
    errno = saved;
    return result;
}
