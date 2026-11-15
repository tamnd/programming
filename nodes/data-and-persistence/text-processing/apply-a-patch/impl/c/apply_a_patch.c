// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

#include "apply_a_patch.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A line of a text: a pointer into the text and a length.
struct span {
    const char *s;
    size_t len;
};

// split_lines splits text after each LF into a new array, and sets *n to the
// number of lines. With keep_lf, each line keeps its LF. A last line with no
// LF is a line too. It returns NULL if there is not enough memory.
static struct span *split_lines(const char *text, size_t *n, int keep_lf) {
    size_t count = 0;
    for (const char *p = text; *p != '\0'; count++) {
        const char *lf = strchr(p, '\n');
        p = lf != NULL ? lf + 1 : p + strlen(p);
    }
    struct span *lines = malloc((count + 1) * sizeof *lines);
    if (lines == NULL) {
        return NULL;
    }
    size_t k = 0;
    for (const char *p = text; *p != '\0'; k++) {
        const char *lf = strchr(p, '\n');
        size_t len = lf != NULL ? (size_t)(lf - p) : strlen(p);
        lines[k].s = p;
        lines[k].len = len + (lf != NULL && keep_lf);
        p += len + (lf != NULL);
    }
    *n = count;
    return lines;
}

// number reads a decimal number at s[*k], before the index end. It returns 0
// if there is no digit or the number is too large.
static int number(const char *s, size_t end, size_t *k, size_t *v) {
    if (*k >= end || s[*k] < '0' || s[*k] > '9') {
        return 0;
    }
    *v = 0;
    for (; *k < end && s[*k] >= '0' && s[*k] <= '9'; (*k)++) {
        size_t d = (size_t)(s[*k] - '0');
        if (*v > (SIZE_MAX - d) / 10) {
            return 0;
        }
        *v = *v * 10 + d;
    }
    return 1;
}

// prefix reads the text t at s[*k], before the index end.
static int prefix(const char *s, size_t end, size_t *k, const char *t) {
    size_t n = strlen(t);
    if (end - *k < n || memcmp(s + *k, t, n) != 0) {
        return 0;
    }
    *k += n;
    return 1;
}

// range reads a start and an optional comma and count. A missing count is 1.
static int range(const char *s, size_t end, size_t *k, size_t *start, size_t *count) {
    if (!number(s, end, k, start)) {
        return 0;
    }
    *count = 1;
    return !prefix(s, end, k, ",") || number(s, end, k, count);
}

// hunk_header reads a line such as @@ -1,3 +1,4 @@ and the text after it.
static int hunk_header(struct span h, size_t *old_start, size_t *old_count, size_t *new_count) {
    size_t k = 0, new_start;
    return prefix(h.s, h.len, &k, "@@ -") && range(h.s, h.len, &k, old_start, old_count) &&
           prefix(h.s, h.len, &k, " +") && range(h.s, h.len, &k, &new_start, new_count) &&
           prefix(h.s, h.len, &k, " @@");
}

static char mark_of(struct span l) { return l.len == 0 ? ' ' : l.s[0]; }

static enum patch_status apply(const struct span *lines, size_t nl, const struct span *p, size_t np, char *out,
                               size_t *line) {
    size_t used = 0, cur = 0, hunks = 0, i = 0;
    while (i < np) {
        if (p[i].len < 2 || p[i].s[0] != '@' || p[i].s[1] != '@') {
            i++;  // a header line, or another line outside a hunk
            continue;
        }
        size_t old_start, old_count, new_count;
        *line = i + 1;
        if (!hunk_header(p[i], &old_start, &old_count, &new_count)) {
            return PATCH_MALFORMED;
        }
        hunks++;
        // An empty range starts after the line old_start.
        if (old_count > 0 && old_start == 0) {
            return PATCH_MALFORMED;
        }
        size_t pos = old_count > 0 ? old_start - 1 : old_start;
        if (pos < cur || pos > nl) {
            return PATCH_MISMATCH;
        }
        for (; cur < pos; cur++) {
            memcpy(out + used, lines[cur].s, lines[cur].len);
            used += lines[cur].len;
        }
        // The first pass checks the marks and the counts, and finds the end.
        size_t first = ++i, old_left = old_count, new_left = new_count, body = 0;
        while (old_left > 0 || new_left > 0) {
            *line = i + 1;
            if (i == np) {
                *line = i;
                return PATCH_MALFORMED;
            }
            char mark = mark_of(p[i]);
            if (mark == '\\') {
                if (body == 0) {
                    return PATCH_MALFORMED;
                }
                i++;
                continue;
            }
            if (mark != ' ' && mark != '-' && mark != '+') {
                return PATCH_MALFORMED;
            }
            if (mark != '+' && old_left-- == 0) {
                return PATCH_MALFORMED;
            }
            if (mark != '-' && new_left-- == 0) {
                return PATCH_MALFORMED;
            }
            body++;
            i++;
        }
        if (i < np && body > 0 && p[i].len > 0 && p[i].s[0] == '\\') {
            i++;
        }
        // The second pass applies the lines. A line before a backslash line
        // has no LF.
        for (size_t k = first; k < i; k++) {
            char mark = mark_of(p[k]);
            if (mark == '\\') {
                continue;
            }
            const char *text = p[k].len > 0 ? p[k].s + 1 : p[k].s;
            size_t tlen = p[k].len > 0 ? p[k].len - 1 : 0;
            int lf = !(k + 1 < i && p[k + 1].len > 0 && p[k + 1].s[0] == '\\');
            if (mark != '+') {
                if (cur == nl || lines[cur].len != tlen + (size_t)lf || memcmp(lines[cur].s, text, tlen) != 0 ||
                    (lf && lines[cur].s[tlen] != '\n')) {
                    *line = k + 1;
                    return PATCH_MISMATCH;
                }
                cur++;
            }
            if (mark != '-') {
                memcpy(out + used, text, tlen);
                used += tlen;
                if (lf) {
                    out[used++] = '\n';
                }
            }
        }
    }
    if (hunks == 0) {
        *line = np;
        return PATCH_MALFORMED;
    }
    for (; cur < nl; cur++) {
        memcpy(out + used, lines[cur].s, lines[cur].len);
        used += lines[cur].len;
    }
    out[used] = '\0';
    return PATCH_OK;
}

enum patch_status apply_patch(const char *old_text, const char *patch, char **out, size_t *line) {
    *out = NULL;
    *line = 0;
    size_t old_len = strlen(old_text), patch_len = strlen(patch);
    // Each byte of the result is a byte of old_text, a byte of a + line of
    // the patch, or the LF of such a line, so the result fits in old_len +
    // patch_len bytes.
    if (old_len > SIZE_MAX - patch_len - 1) {
        return PATCH_NOMEM;
    }
    size_t nl, np;
    struct span *lines = split_lines(old_text, &nl, 1);
    struct span *p = split_lines(patch, &np, 0);
    char *result = malloc(old_len + patch_len + 1);
    enum patch_status st = PATCH_NOMEM;
    if (lines != NULL && p != NULL && result != NULL) {
        st = apply(lines, nl, p, np, result, line);
    }
    free(lines);
    free(p);
    if (st == PATCH_OK) {
        *out = result;
    } else {
        free(result);
    }
    return st;
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

enum patch_status apply_patch_file(const char *path, const char *patch_path, char **out, size_t *line) {
    *out = NULL;
    *line = 0;
    char *old_text = read_file(path);
    if (old_text == NULL) {
        return errno == ENOMEM ? PATCH_NOMEM : PATCH_IO;
    }
    char *patch = read_file(patch_path);
    if (patch == NULL) {
        int saved = errno;
        free(old_text);
        errno = saved;
        return saved == ENOMEM ? PATCH_NOMEM : PATCH_IO;
    }
    enum patch_status st = apply_patch(old_text, patch, out, line);
    free(old_text);
    free(patch);
    return st;
}
