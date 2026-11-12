// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

#ifndef COPY_BUFFERED_H
#define COPY_BUFFERED_H

// copy_buffered copies the file at src to the file at dst through stdio
// buffers of size bytes, and returns the number of bytes copied. On an error
// it returns -1 and sets errno: EINVAL if size is below 1, or the error of
// fopen, fread, fwrite or fclose. An error from the last write comes from
// fclose, because fclose writes the bytes that are still in the buffer.
long long copy_buffered(const char *src, const char *dst, long size);

#endif
