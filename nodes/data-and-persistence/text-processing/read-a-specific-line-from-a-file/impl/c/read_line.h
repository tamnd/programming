// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

#ifndef READ_LINE_H
#define READ_LINE_H

#include <stddef.h>

// read_line reads line number of the file at path. Line 1 is the first line.
// A line ends at LF, and the LF is not part of it. A last line with no LF is a
// line too. It reads the file only up to the line.
//
// It returns 1 and puts the line into *line and its length into *len: the
// line is a new C string, which the caller frees. It returns 0 if there is no
// such line. It returns -1 and sets errno if it cannot read the file: ENOENT
// if the file does not exist.
int read_line(const char *path, int number, char **line, size_t *len);

#endif
