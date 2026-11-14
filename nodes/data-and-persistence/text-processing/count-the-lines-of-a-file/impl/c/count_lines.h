// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

#ifndef COUNT_LINES_H
#define COUNT_LINES_H

#include <stdint.h>

// count_lines reads the file at path as bytes. It stores the number of LF
// bytes in *newlines, the number that wc -l prints, and the number of lines in
// *lines, which also counts a last line that has no LF. It returns 0. On an
// error it returns -1, and errno holds the error of fopen or fread.
int count_lines(const char *path, uintmax_t *newlines, uintmax_t *lines);

#endif
