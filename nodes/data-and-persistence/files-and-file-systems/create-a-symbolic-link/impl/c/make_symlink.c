// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

#define _POSIX_C_SOURCE 200809L

#include "make_symlink.h"

#include <errno.h>
#include <unistd.h>

ssize_t make_symlink(const char *target, const char *link, char *buf, size_t size) {
    if (symlink(target, link) != 0) return -1;
    if (size == 0) {
        errno = ERANGE;
        return -1;
    }
    // readlink does not end the target with a NUL byte. When it fills the
    // buffer, the target can be longer, so the code needs one byte more.
    ssize_t n = readlink(link, buf, size);
    if (n < 0) return -1;
    if ((size_t)n == size) {
        errno = ERANGE;
        return -1;
    }
    buf[n] = '\0';
    return n;
}
