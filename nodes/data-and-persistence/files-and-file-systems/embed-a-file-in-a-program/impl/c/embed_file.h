/* SPDX-License-Identifier: MIT */
/* Part of https://programming.tamnd.com/embed-a-file-in-a-program/. */

#ifndef EMBED_FILE_H
#define EMBED_FILE_H

#include <stddef.h>

/* The bytes of lines.txt, put in the program when it is compiled. */
const unsigned char *embedded_data(size_t *len);

/* Returns a pointer to the start of line n of lines.txt (1 is the line after
   the license header)
   and sets *len to its length with no line end. The line is not followed by
   a 0 byte. Returns NULL when there is no such line. */
const char *embedded_line(long n, size_t *len);

#endif
