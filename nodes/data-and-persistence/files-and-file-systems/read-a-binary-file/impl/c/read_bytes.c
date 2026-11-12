// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

// fileno and fstat are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "read_bytes.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// open_file opens path for binary reading and rejects a directory. On
// Windows, "rb" stops the C library from changing CR LF and from taking
// CTRL+Z as the end of the file; on POSIX systems the b has no effect.
static FILE *open_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) return NULL;
    struct stat st;
    int err = 0;
    if (fstat(fileno(f), &st) != 0) {
        err = errno;
    } else if (S_ISDIR(st.st_mode)) {
        err = EISDIR;
    }
    if (err != 0) {
        fclose(f);
        errno = err;
        return NULL;
    }
    return f;
}

unsigned char *read_bytes(const char *path, size_t *len) {
    FILE *f = open_file(path);
    if (f == NULL) return NULL;

    int err = 0;
    size_t n = 0, cap = 4096;
    unsigned char *buf = malloc(cap);
    while (buf != NULL) {
        // fread gives fewer bytes than it was asked for only at the end of
        // the file or on an error. While the buffer fills, make it bigger.
        n += fread(buf + n, 1, cap - n, f);
        if (n < cap) break;
        unsigned char *bigger = realloc(buf, cap * 2);
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

enum read_exact_result read_exact(const char *path, unsigned char *buf, size_t n) {
    FILE *f = open_file(path);
    if (f == NULL) return READ_EXACT_ERROR;
    // POSIX says that fread gives fewer than n bytes only at the end of
    // the file or on an error, so one call is enough; ferror tells which.
    size_t got = fread(buf, 1, n, f);
    enum read_exact_result result = READ_EXACT_OK;
    int err = 0;
    if (got < n) {
        if (ferror(f)) {
            err = errno != 0 ? errno : EIO;
            result = READ_EXACT_ERROR;
        } else {
            result = READ_EXACT_EOF;
        }
    }
    fclose(f);
    if (err != 0) errno = err;
    return result;
}
