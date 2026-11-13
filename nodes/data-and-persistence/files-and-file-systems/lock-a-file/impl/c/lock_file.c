// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

// flock is not in POSIX, but Linux and the BSDs (with macOS) have it.
#define _DEFAULT_SOURCE
#define _DARWIN_C_SOURCE

#include "lock_file.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

int try_lock_file(const char *path, enum lock_mode mode) {
    if (mode != LOCK_MODE_SHARED && mode != LOCK_MODE_EXCLUSIVE) {
        errno = EINVAL;
        return -1;
    }
    // flock locks belong to the open file, so each call opens the file again.
    // Without O_CREAT, a missing file is an error.
    int fd = open(path, O_RDWR);
    if (fd < 0) return -1;
    int op = mode == LOCK_MODE_SHARED ? LOCK_SH : LOCK_EX;
    while (flock(fd, op | LOCK_NB) != 0) {
        if (errno == EINTR) continue;
        int e = errno;
        close(fd);
        errno = e;
        return -1;
    }
    return fd;
}

int unlock_file(int fd) {
    int rc = flock(fd, LOCK_UN);
    int e = errno;
    // The close also releases the lock, so the close runs after an error too.
    if (close(fd) != 0 && rc == 0) return -1;
    errno = e;
    return rc;
}

int try_lock(const char *path, enum lock_mode held, enum lock_mode want) {
    int first = -1;
    if (held != LOCK_MODE_NONE && (first = try_lock_file(path, held)) < 0) return -1;
    int result;
    int second = try_lock_file(path, want);
    if (second >= 0) {
        result = unlock_file(second) == 0 ? 1 : -1;
    } else {
        result = errno == EWOULDBLOCK ? 0 : -1;
    }
    int e = errno;
    if (first >= 0) unlock_file(first);
    errno = e;
    return result;
}
