// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

#ifndef COPY_FILE_H
#define COPY_FILE_H

// copy_file copies the bytes and the permission bits of the file at src to
// dst, and replaces a file at dst. It returns 0, or -1 with errno: ENOENT,
// EISDIR for a directory at src or dst, EINVAL when src and dst are the same
// file, or the error of open, read, write or close.
int copy_file(const char *src, const char *dst);

#endif
