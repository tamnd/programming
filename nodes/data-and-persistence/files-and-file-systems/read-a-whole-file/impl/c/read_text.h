// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

#ifndef READ_TEXT_H
#define READ_TEXT_H

#include <stddef.h>

// read_text reads all of the UTF-8 text file at path. It returns a new
// buffer that the caller must free, with a 0 byte after the text, and stores
// the number of bytes of the text in *len. The text can hold 0 bytes, so use
// *len and not strlen. On an error it returns NULL and sets errno: EISDIR for
// a directory, EILSEQ for bytes that are not valid UTF-8, or the error of
// fopen, fstat or fread.
char *read_text(const char *path, size_t *len);

#endif
