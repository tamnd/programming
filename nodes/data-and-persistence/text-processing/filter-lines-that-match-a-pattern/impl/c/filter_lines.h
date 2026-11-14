// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

#ifndef FILTER_LINES_H
#define FILTER_LINES_H

#include <stddef.h>

// A line of the file: text is a C string with no LF, and len is its length.
struct line {
    const char *text;
    size_t len;
};

// The lines that matched. data holds all the bytes of the file, and each line
// points into it.
struct matched_lines {
    struct line *lines;
    size_t len;
    char *data;
};

// filter_lines reads the file at path and puts the lines that match pattern,
// a POSIX extended regular expression, into out. A line matches if the
// pattern matches some part of it. A line ends at LF, and the LF is not part
// of it. A last line with no LF is a line too. It returns 0, or -1 and sets
// errno: ENOENT if the file does not exist, EINVAL if the pattern is not
// valid.
int filter_lines(const char *path, const char *pattern, struct matched_lines *out);

// matched_lines_free frees the memory of the result.
void matched_lines_free(struct matched_lines *m);

#endif
