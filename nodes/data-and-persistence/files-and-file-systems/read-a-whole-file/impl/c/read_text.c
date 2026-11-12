// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

// fileno and fstat are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "read_text.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// utf8_valid checks that s[0..n) is valid UTF-8 (RFC 3629). It rejects
// overlong forms, surrogates and values above U+10FFFF.
static bool utf8_valid(const unsigned char *s, size_t n) {
    for (size_t i = 0; i < n;) {
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
    return true;
}

char *read_text(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) return NULL;

    int err = 0;
    char *buf = NULL;
    size_t n = 0;
    struct stat st;
    if (fstat(fileno(f), &st) != 0) {
        err = errno;
    } else if (S_ISDIR(st.st_mode)) {
        err = EISDIR;
    } else {
        // The size from fstat is only a first guess: the file can change
        // while it is read, and POSIX does not say what st_size means for
        // most types of file other than a regular file. So read until fread
        // gives less than it was asked for.
        size_t cap = st.st_size > 0 ? (size_t)st.st_size + 1 : 4096;
        buf = malloc(cap);
        while (buf != NULL) {
            n += fread(buf + n, 1, cap - n, f);
            if (n < cap) break; // the end of the file, or an error
            char *bigger = realloc(buf, cap * 2);
            if (bigger == NULL) {
                free(buf);
                buf = NULL;
                break;
            }
            buf = bigger;
            cap *= 2;
        }
        if (buf == NULL) {
            err = ENOMEM;
        } else if (ferror(f)) {
            err = errno != 0 ? errno : EIO;
        } else if (!utf8_valid((const unsigned char *)buf, n)) {
            err = EILSEQ;
        } else {
            buf[n] = '\0'; // n < cap, so there is room
        }
    }
    fclose(f);
    if (err != 0) {
        free(buf);
        errno = err;
        return NULL;
    }
    *len = n;
    return buf;
}
