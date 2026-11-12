// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

#ifndef READ_BYTES_H
#define READ_BYTES_H

#include <stddef.h>

// read_bytes reads all of the bytes of the file at path. It returns a new
// buffer that the caller must free, and stores the number of bytes in *len.
// The bytes can hold 0 bytes, so use *len and not strlen. For an empty file
// it returns a buffer that is not NULL and sets *len to 0. On an error it
// returns NULL and sets errno: EISDIR for a directory, or the error of
// fopen, fstat or fread.
unsigned char *read_bytes(const char *path, size_t *len);

// The results of read_exact.
enum read_exact_result {
    READ_EXACT_OK = 0,     // buf holds the first n bytes of the file
    READ_EXACT_ERROR = -1, // an error; errno tells which
    READ_EXACT_EOF = -2,   // the file has fewer than n bytes
};

// read_exact reads the first n bytes of the file at path into buf, which
// must have room for n bytes. On READ_EXACT_ERROR, errno is EISDIR for a
// directory or the error of fopen, fstat or fread. On an error or
// READ_EXACT_EOF, the contents of buf are not specified.
enum read_exact_result read_exact(const char *path, unsigned char *buf, size_t n);

#endif
