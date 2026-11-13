// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

#ifndef FILE_SIZE_H
#define FILE_SIZE_H

#include <sys/types.h>

// file_size stores the size in bytes of the regular file at path in *size.
// stat follows a symbolic link, so a link gives the size of its target. It
// returns 0, or -1 with errno: ENOENT for a missing file, and EISDIR for a
// directory, because the size of a directory is not the size of a file.
int file_size(const char *path, off_t *size);

#endif
