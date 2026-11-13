// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

#ifndef SET_MODE_H
#define SET_MODE_H

#include <sys/types.h>

// set_mode sets the mode of the file at path to mode, a string of three or
// four octal digits, as for the chmod command, and stores the new permission
// bits (st_mode & 07777) in *result. chmod follows a symbolic link, and the
// umask does not apply. It returns 0, or -1 with errno: EINVAL for a mode that
// is not three or four octal digits, and ENOENT for a missing file.
int set_mode(const char *path, const char *mode, mode_t *result);

#endif
