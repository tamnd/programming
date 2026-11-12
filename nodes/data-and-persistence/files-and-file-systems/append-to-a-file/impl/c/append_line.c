// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

// open, write and O_CLOEXEC are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "append_line.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int append_line(const char *path, const char *line) {
    size_t n = strlen(line);
    if (strpbrk(line, "\r\n") != NULL) {
        errno = EINVAL;
        return -1;
    }
    // Put the line and its '\n' in one buffer, so that one write sends both.
    char *data = malloc(n + 1);
    if (data == NULL) return -1;
    memcpy(data, line, n);
    data[n] = '\n';

    // O_APPEND moves the offset to the end of the file before each write.
    int fd = open(path, O_WRONLY | O_APPEND | O_CREAT | O_CLOEXEC, 0666);
    if (fd < 0) {
        free(data);
        return -1;
    }
    ssize_t written;
    do {
        written = write(fd, data, n + 1);
    } while (written < 0 && errno == EINTR);
    int err = written < 0 ? errno : (size_t)written != n + 1 ? EIO : 0;
    free(data);
    if (close(fd) != 0 && err == 0) err = errno;
    if (err != 0) {
        errno = err;
        return -1;
    }
    return 0;
}
