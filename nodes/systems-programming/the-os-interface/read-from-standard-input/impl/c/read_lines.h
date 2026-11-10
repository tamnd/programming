// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

#ifndef READ_LINES_H
#define READ_LINES_H

#include <stddef.h>
#include <stdio.h>

// read_lines reads stream to its end. It stores a new array of *count lines
// in *lines and returns 0. Each line is a string without its "\n" or "\r\n".
// On a read error or when memory runs out, it returns -1 and sets errno.
// Free the result with free_lines. Pass stdin to read standard input.
int read_lines(FILE *stream, char ***lines, size_t *count);

// free_lines frees the array of read_lines and each of its lines.
void free_lines(char **lines, size_t count);

#endif
