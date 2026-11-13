// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

#define _POSIX_C_SOURCE 200809L

#include "set_mode.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

int set_mode(const char *path, const char *mode, mode_t *result) {
    size_t n = mode == NULL ? 0 : strlen(mode);
    if (n < 3 || n > 4) {
        errno = EINVAL;
        return -1;
    }
    // strtol would accept a sign, spaces and a newline, so the code reads
    // each digit itself.
    mode_t m = 0;
    for (size_t i = 0; i < n; i++) {
        if (mode[i] < '0' || mode[i] > '7') {
            errno = EINVAL;
            return -1;
        }
        m = (mode_t)(m * 8 + (mode_t)(mode[i] - '0'));
    }
    struct stat st;
    if (chmod(path, m) != 0 || stat(path, &st) != 0) return -1;
    *result = st.st_mode & 07777;
    return 0;
}
