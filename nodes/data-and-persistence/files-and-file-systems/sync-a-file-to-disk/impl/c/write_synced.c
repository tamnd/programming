// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

#define _POSIX_C_SOURCE 200809L
#define _DARWIN_C_SOURCE // F_FULLFSYNC on macOS

#include "write_synced.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// sync_fd sends the data of fd to the device. On macOS, fsync leaves the data
// in the cache of the drive, and F_FULLFSYNC asks the drive to write it. Some
// file systems do not support F_FULLFSYNC, so fsync is the fallback.
static int sync_fd(int fd) {
#ifdef F_FULLFSYNC
    if (fcntl(fd, F_FULLFSYNC) == 0) return 0;
#endif
    return fsync(fd);
}

int sync_directory(const char *dir) {
    int fd = open(dir, O_RDONLY | O_DIRECTORY);
    if (fd < 0) return -1;
    if (fsync(fd) != 0) {
        int e = errno;
        close(fd);
        errno = e;
        return -1;
    }
    return close(fd);
}

int write_synced(const char *path, const char *text, size_t len) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    // write can write fewer bytes than asked, so it runs in a loop.
    for (size_t done = 0; done < len;) {
        ssize_t n = write(fd, text + done, len - done);
        if (n < 0) {
            if (errno == EINTR) continue;
            int e = errno;
            close(fd);
            errno = e;
            return -1;
        }
        done += (size_t)n;
    }
    if (sync_fd(fd) != 0) {
        int e = errno;
        close(fd);
        errno = e;
        return -1;
    }
    if (close(fd) != 0) return -1;

    // The directory of path: the part before the last slash, or ".".
    const char *slash = strrchr(path, '/');
    if (slash == NULL) return sync_directory(".");
    if (slash == path) return sync_directory("/");
    size_t n = (size_t)(slash - path);
    char *dir = malloc(n + 1);
    if (dir == NULL) return -1;
    memcpy(dir, path, n);
    dir[n] = '\0';
    int rc = sync_directory(dir);
    int e = errno;
    free(dir);
    errno = e;
    return rc;
}
