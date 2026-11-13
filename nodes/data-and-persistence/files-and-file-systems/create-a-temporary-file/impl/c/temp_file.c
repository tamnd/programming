// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

#define _POSIX_C_SOURCE 200809L

#include "temp_file.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// mkstemp replaces the six X characters at the end of the template, and it
// opens the file with O_CREAT | O_EXCL and the mode 0600 in one call.
static int open_temp(const char *dir, const char *prefix, char *path, size_t size) {
    if (strchr(prefix, '/') != NULL) {
        errno = EINVAL;
        return -1;
    }
    int n = snprintf(path, size, "%s/%sXXXXXX", dir, prefix);
    if (n < 0 || (size_t)n >= size) {
        errno = ERANGE;
        return -1;
    }
    return mkstemp(path);
}

int make_temp_file(const char *dir, const char *prefix, char *path, size_t size) {
    int fd = open_temp(dir, prefix, path, size);
    if (fd < 0) return -1;
    if (close(fd) != 0) {
        int e = errno;
        unlink(path);
        errno = e;
        return -1;
    }
    return 0;
}

char *use_temp_file(const char *dir, const char *prefix, const char *data) {
    char path[4096];
    int fd = open_temp(dir, prefix, path, sizeof path);
    if (fd < 0) return NULL;
    // The descriptor of mkstemp is open for reading and writing, so the
    // function writes and reads through it, and the name is not opened again.
    size_t len = strlen(data);
    char *text = malloc(len + 1);
    int e = 0;
    size_t done = 0;
    if (text == NULL) e = ENOMEM;
    while (e == 0 && done < len) {
        ssize_t w = write(fd, data + done, len - done);
        if (w < 0) e = errno;
        else done += (size_t)w;
    }
    if (e == 0 && lseek(fd, 0, SEEK_SET) < 0) e = errno;
    done = 0;
    while (e == 0 && done < len) {
        ssize_t r = read(fd, text + done, len - done);
        if (r < 0) e = errno;
        else if (r == 0) e = EIO;
        else done += (size_t)r;
    }
    if (close(fd) != 0 && e == 0) e = errno;
    if (unlink(path) != 0 && e == 0) e = errno;
    if (e != 0) {
        free(text);
        errno = e;
        return NULL;
    }
    text[len] = '\0';
    return text;
}
