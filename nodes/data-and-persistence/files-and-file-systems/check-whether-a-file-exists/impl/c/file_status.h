// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

#ifndef FILE_STATUS_H
#define FILE_STATUS_H

// file_status tells what is at path: "file", "directory", "other", "missing"
// or "broken-link". It follows symbolic links. On an error other than "no
// such entry", for example EACCES, it returns NULL and keeps errno from stat.
const char *file_status(const char *path);

#endif
