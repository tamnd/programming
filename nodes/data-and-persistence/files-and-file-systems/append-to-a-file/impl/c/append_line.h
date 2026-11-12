// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

#ifndef APPEND_LINE_H
#define APPEND_LINE_H

// append_line adds line and one '\n' to the end of the file at path, in one
// write call, and makes the file if it is missing. It returns 0. On an error
// it returns -1 and sets errno: EINVAL if line holds '\n' or '\r', EIO for a
// short write, or the error of open, write or close.
int append_line(const char *path, const char *line);

#endif
