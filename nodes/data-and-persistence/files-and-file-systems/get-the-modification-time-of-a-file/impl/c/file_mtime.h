// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

#ifndef FILE_MTIME_H
#define FILE_MTIME_H

#include <time.h>

// file_mtime stores the modification time of the file at path in *mtime, in
// whole seconds since 1970-01-01T00:00:00Z. stat follows a symbolic link. It
// returns 0, or -1 with errno, for example ENOENT for a missing file.
int file_mtime(const char *path, time_t *mtime);

#endif
