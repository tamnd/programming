// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

#ifndef RENAME_FILE_H
#define RENAME_FILE_H

// rename_file gives the file at src the name dst, and replaces a file at dst.
// It returns 0, or -1 with errno from rename, for example ENOENT, EISDIR, or
// EXDEV when the two paths are on different file systems.
int rename_file(const char *src, const char *dst);

#endif
