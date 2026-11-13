// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

#ifndef TRUNCATE_FILE_H
#define TRUNCATE_FILE_H

#include <sys/types.h>

// truncate_file sets the size of the existing file at path to length bytes.
// It returns 0, or -1 with errno from truncate: ENOENT, EISDIR, or EINVAL for
// a length under 0.
int truncate_file(const char *path, off_t length);

#endif
