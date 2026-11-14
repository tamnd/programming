// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

#ifndef SORT_LINES_H
#define SORT_LINES_H

#include <stddef.h>

struct line {
    const char *text;  // the bytes of the line, with no LF, ended by a NUL
    size_t len;        // the number of bytes, not counting the NUL
};

struct sorted_lines {
    struct line *lines;  // the lines in byte order, or NULL if there are none
    size_t len;          // the number of lines
    char *data;          // the bytes of the file, which the lines point into
};

// sort_lines reads the file at path, splits it into lines at each LF, and
// sorts the lines by their bytes, which for UTF-8 is the order of the code
// points. A last line with no LF is a line too. It fills *out and returns 0.
// Free the result with sort_lines_free. On an error it returns -1 and sets
// errno: the error of fopen, EIO for a read error, or ENOMEM.
int sort_lines(const char *path, struct sorted_lines *out);

// sort_lines_free frees the memory of a result of sort_lines.
void sort_lines_free(struct sorted_lines *s);

#endif
