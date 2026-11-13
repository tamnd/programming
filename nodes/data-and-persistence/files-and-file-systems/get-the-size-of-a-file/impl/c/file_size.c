// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

#define _POSIX_C_SOURCE 200809L

#include "file_size.h"

#include <errno.h>
#include <sys/stat.h>

int file_size(const char *path, off_t *size) {
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    if (S_ISDIR(st.st_mode)) {
        errno = EISDIR;
        return -1;
    }
    *size = st.st_size;
    return 0;
}
