// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

#ifndef LINE_STATS_H
#define LINE_STATS_H

#include <stddef.h>

// line_stats reads the UTF-8 text file at path line by line. It stores the
// number of lines in *lines and the number of code points of the longest line
// in *longest, and returns 0. On an error it returns -1 and sets errno:
// EILSEQ for bytes that are not valid UTF-8, or the error of fopen or getline.
int line_stats(const char *path, size_t *lines, size_t *longest);

#endif
