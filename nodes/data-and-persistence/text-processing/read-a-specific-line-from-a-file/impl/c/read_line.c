// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

// getline is in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "read_line.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int read_line(const char *path, int number, char **line, size_t *len) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    char *buf = NULL;
    size_t cap = 0;
    ssize_t n = -1;
    // getline reads up to and with the next LF, and grows buf as necessary.
    // It returns -1 at the end of the file, or on an error.
    for (int i = 0; i < number; i++) {
        n = getline(&buf, &cap, f);
        if (n < 0) {
            break;
        }
    }
    int failed = ferror(f);
    int e = errno;
    fclose(f);
    if (failed) {
        free(buf);
        errno = e;
        return -1;
    }
    if (number < 1 || n < 0) {
        free(buf);
        return 0;
    }
    if (n > 0 && buf[n - 1] == '\n') {
        buf[--n] = '\0';
    }
    *line = buf;
    *len = (size_t)n;
    return 1;
}
