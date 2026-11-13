// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

#define _POSIX_C_SOURCE 200809L

#include "file_mtime.h"

#include <sys/stat.h>

int file_mtime(const char *path, time_t *mtime) {
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    // st_mtime is the tv_sec part of the time. It is the floor, because the
    // tv_nsec part is never negative, also before 1970.
    *mtime = st.st_mtime;
    return 0;
}
