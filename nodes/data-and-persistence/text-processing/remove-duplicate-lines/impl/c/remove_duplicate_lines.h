// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

#ifndef REMOVE_DUPLICATE_LINES_H
#define REMOVE_DUPLICATE_LINES_H

#include <stddef.h>

struct line {
    const char *text;  // the bytes of the line, with no LF, ended by a NUL
    size_t len;        // the number of bytes, not counting the NUL
};

struct unique_lines {
    struct line *lines;  // the first copy of each line, in file order, or NULL
    size_t len;          // the number of lines
    char *data;          // the bytes of the file, which the lines point into
};

// remove_duplicate_lines reads the file at path, splits it into lines at each
// LF, and keeps the first copy of each line, in the order of the file. A last
// line with no LF is a line too. It fills *out and returns 0. Free the result
// with unique_lines_free. On an error it returns -1 and sets errno: the error
// of fopen, EIO for a read error, or ENOMEM.
int remove_duplicate_lines(const char *path, struct unique_lines *out);

// unique_lines_free frees the memory of a result of remove_duplicate_lines.
void unique_lines_free(struct unique_lines *u);

#endif
