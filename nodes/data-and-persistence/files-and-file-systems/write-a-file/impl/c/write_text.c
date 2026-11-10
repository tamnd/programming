// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

#define _POSIX_C_SOURCE 200809L

#include "write_text.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// "wb" truncates or creates; "ab" appends or creates. The b means no change
// to the line ends on any system.
static int write_stdio(const char *path, const char *text, size_t len, const char *how) {
    FILE *f = fopen(path, how);
    if (f == NULL) {
        return -1;
    }
    size_t n = fwrite(text, 1, len, f);
    int saved = errno;
    // fclose flushes the buffer, so it can report a write error too.
    if (fclose(f) != 0) {
        return -1;
    }
    if (n != len) {
        errno = saved;
        return -1;
    }
    return 0;
}

static int write_all(int fd, const char *text, size_t len) {
    while (len > 0) {
        ssize_t n = write(fd, text, len);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        text += n;
        len -= (size_t)n;
    }
    return 0;
}

static int replace(const char *path, const char *text, size_t len) {
    // The temporary file is in the same directory, so the rename does not
    // cross a file system.
    size_t plen = strlen(path);
    char *tmp = malloc(plen + sizeof ".XXXXXX");
    if (tmp == NULL) {
        return -1;
    }
    memcpy(tmp, path, plen);
    memcpy(tmp + plen, ".XXXXXX", sizeof ".XXXXXX");
    int fd = mkstemp(tmp);
    if (fd < 0) {
        free(tmp);
        return -1;
    }
    int failed = write_all(fd, text, len) != 0 || fsync(fd) != 0;
    int saved = errno;
    if (close(fd) != 0 && !failed) {
        failed = 1;
        saved = errno;
    }
    if (!failed && rename(tmp, path) != 0) {
        failed = 1;
        saved = errno;
    }
    if (failed) {
        unlink(tmp);
        errno = saved;
    }
    free(tmp);
    return failed ? -1 : 0;
}

int write_text(const char *path, const char *text, size_t len, enum write_mode mode) {
    switch (mode) {
    case WRITE_TRUNCATE:
        return write_stdio(path, text, len, "wb");
    case WRITE_APPEND:
        return write_stdio(path, text, len, "ab");
    case WRITE_ATOMIC:
        return replace(path, text, len);
    }
    errno = EINVAL;
    return -1;
}
