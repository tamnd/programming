// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

// truncate is in POSIX, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "truncate_file.h"

#include <unistd.h>

// POSIX truncate cuts the bytes after length, or adds zero bytes up to
// length. It does not make a missing file.
int truncate_file(const char *path, off_t length) { return truncate(path, length) == 0 ? 0 : -1; }
