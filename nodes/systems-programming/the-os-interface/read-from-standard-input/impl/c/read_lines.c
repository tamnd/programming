// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

// getline is in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "read_lines.h"

#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>

void free_lines(char **lines, size_t count) {
    for (size_t i = 0; i < count; i++) free(lines[i]);
    free(lines);
}

int read_lines(FILE *stream, char ***lines, size_t *count) {
    char **out = NULL;
    size_t n = 0, cap = 0;
    for (;;) {
        char *line = NULL;
        size_t size = 0;
        errno = 0;
        // getline returns -1 at the end of input and on an error.
        ssize_t len = getline(&line, &size, stream);
        if (len == -1) {
            free(line);
            break;
        }
        if (len > 0 && line[len - 1] == '\n') line[--len] = '\0';
        if (len > 0 && line[len - 1] == '\r') line[--len] = '\0';
        if (n == cap) {
            size_t new_cap = cap == 0 ? 16 : cap * 2;
            char **grown = realloc(out, new_cap * sizeof *grown);
            if (grown == NULL) {
                free(line);
                free_lines(out, n);
                return -1;
            }
            out = grown;
            cap = new_cap;
        }
        out[n++] = line;
    }
    // feof tells the end of input apart from an error.
    if (ferror(stream)) {
        int err = errno != 0 ? errno : EIO;
        free_lines(out, n);
        errno = err;
        return -1;
    }
    *lines = out;
    *count = n;
    return 0;
}
