// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

// getline is in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "line_stats.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// utf8_count checks that s[0..n) is valid UTF-8 (RFC 3629) and stores the
// number of code points in *count. It rejects overlong forms, surrogates and
// values above U+10FFFF.
static bool utf8_count(const unsigned char *s, size_t n, size_t *count) {
    size_t k = 0;
    for (size_t i = 0; i < n; k++) {
        unsigned char c = s[i];
        size_t len;
        unsigned char lo = 0x80, hi = 0xBF; // the range of the second byte
        if (c < 0x80) {
            i++;
            continue;
        } else if (c >= 0xC2 && c <= 0xDF) {
            len = 2;
        } else if (c >= 0xE0 && c <= 0xEF) {
            len = 3;
            if (c == 0xE0) lo = 0xA0; // no overlong form
            if (c == 0xED) hi = 0x9F; // no surrogate
        } else if (c >= 0xF0 && c <= 0xF4) {
            len = 4;
            if (c == 0xF0) lo = 0x90; // no overlong form
            if (c == 0xF4) hi = 0x8F; // no value above U+10FFFF
        } else {
            return false;
        }
        if (n - i < len || s[i + 1] < lo || s[i + 1] > hi) return false;
        for (size_t j = 2; j < len; j++) {
            if ((s[i + j] & 0xC0) != 0x80) return false;
        }
        i += len;
    }
    *count = k;
    return true;
}

int line_stats(const char *path, size_t *lines, size_t *longest) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) return -1;

    char *line = NULL;
    size_t cap = 0;
    size_t count = 0, max = 0;
    ssize_t len;
    int err = 0;
    // getline returns -1 at the end of the file and on an error.
    while ((len = getline(&line, &cap, f)) != -1) {
        if (len > 0 && line[len - 1] == '\n') len--;
        if (len > 0 && line[len - 1] == '\r') len--;
        size_t cps;
        if (!utf8_count((const unsigned char *)line, (size_t)len, &cps)) {
            err = EILSEQ;
            break;
        }
        count++;
        if (cps > max) max = cps;
    }
    if (err == 0 && ferror(f)) err = errno;
    free(line);
    fclose(f);
    if (err != 0) {
        errno = err;
        return -1;
    }
    *lines = count;
    *longest = max;
    return 0;
}
