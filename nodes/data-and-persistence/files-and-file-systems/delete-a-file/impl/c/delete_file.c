// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

// unlink is in POSIX, not in ISO C. ISO C remove also deletes an empty
// directory, so it is not used.
#define _POSIX_C_SOURCE 200809L

#include "delete_file.h"

#include <errno.h>
#include <unistd.h>

int delete_file(const char *path) {
    if (unlink(path) == 0) return 1;
    if (errno == ENOENT || errno == ENOTDIR) return 0;
    return -1;
}
