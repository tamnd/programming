// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

#ifndef FIRST_AND_LAST_LINES_H
#define FIRST_AND_LAST_LINES_H

#include <stddef.h>

// A line of the file: text is a C string with no LF, and len is its length.
struct line {
    const char *text;
    size_t len;
};

// The first lines and the last lines of a file. first and last point into
// lines, which holds all the lines of the file, and each line points into
// data, which holds all the bytes of the file. The two parts can overlap.
struct first_and_last {
    const struct line *first;
    size_t first_len;
    const struct line *last;
    size_t last_len;
    struct line *lines;
    char *data;
};

// first_and_last_lines reads the file at path and puts its first count lines
// and its last count lines into out. If the file has fewer than count lines,
// each part has all the lines. A line ends at LF, and the LF is not part of
// it. A last line with no LF is a line too. It returns 0, or -1 and sets
// errno: ENOENT if the file does not exist.
int first_and_last_lines(const char *path, size_t count, struct first_and_last *out);

// first_and_last_free frees the memory of the result.
void first_and_last_free(struct first_and_last *r);

#endif
