// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

#ifndef TEMP_FILE_H
#define TEMP_FILE_H

#include <stddef.h>

// make_temp_file makes a new, empty file in dir whose name is prefix followed
// by six random characters, with the mode 0600, and writes its path to path,
// which has room for size bytes. It returns 0, or -1 with errno: EINVAL when
// prefix has a slash, ERANGE when the path does not fit, or the errno of
// mkstemp, such as ENOENT when dir does not exist.
int make_temp_file(const char *dir, const char *prefix, char *path, size_t size);

// use_temp_file writes data to a new temporary file, reads it back, removes
// the file, and returns the text in a new string that the caller frees. It
// returns NULL with errno on an error.
char *use_temp_file(const char *dir, const char *prefix, const char *data);

#endif
