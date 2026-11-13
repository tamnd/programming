// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

// stat and lstat are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "file_status.h"

#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>

const char *file_status(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        // ENOTDIR: a part of the path before the last one is not a directory.
        if (errno != ENOENT && errno != ENOTDIR) return NULL;
        if (lstat(path, &st) == 0 && S_ISLNK(st.st_mode)) return "broken-link";
        return "missing";
    }
    if (S_ISREG(st.st_mode)) return "file";
    if (S_ISDIR(st.st_mode)) return "directory";
    return "other";
}
