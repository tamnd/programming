// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

// open, read, write, fstat and fchmod are in POSIX, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "copy_file.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// copy_all writes all bytes from in to out. read and write can do less than
// the full count, and a signal can stop them with EINTR, so both are loops.
static int copy_all(int in, int out) {
    static char buf[1 << 16];
    for (;;) {
        ssize_t n = read(in, buf, sizeof buf);
        if (n == 0) return 0;
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        for (ssize_t off = 0; off < n;) {
            ssize_t w = write(out, buf + off, (size_t)(n - off));
            if (w < 0) {
                if (errno == EINTR) continue;
                return -1;
            }
            off += w;
        }
    }
}

int copy_file(const char *src, const char *dst) {
    int in = open(src, O_RDONLY);
    if (in < 0) return -1;
    struct stat s, d;
    int err = 0;
    if (fstat(in, &s) != 0) {
        err = errno;
    } else if (S_ISDIR(s.st_mode)) {
        err = EISDIR;
    } else if (stat(dst, &d) == 0) {
        // O_TRUNC on the same file makes it empty before the copy reads it.
        if (S_ISDIR(d.st_mode)) err = EISDIR;
        else if (d.st_dev == s.st_dev && d.st_ino == s.st_ino) err = EINVAL;
    }
    if (err != 0) {
        close(in);
        errno = err;
        return -1;
    }
    mode_t mode = s.st_mode & 07777;
    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (out < 0) {
        err = errno;
        close(in);
        errno = err;
        return -1;
    }
    // The mode of open is used only for a new file, and the umask changes
    // it, so fchmod sets the bits.
    if (copy_all(in, out) != 0 || fchmod(out, mode) != 0) err = errno;
    close(in);
    // A write error can come only at the close, so its result is checked.
    if (close(out) != 0 && err == 0) err = errno;
    if (err != 0) {
        errno = err;
        return -1;
    }
    return 0;
}
