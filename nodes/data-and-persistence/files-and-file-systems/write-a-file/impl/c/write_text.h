// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

#ifndef WRITE_TEXT_H
#define WRITE_TEXT_H

#include <stddef.h>

enum write_mode { WRITE_TRUNCATE, WRITE_APPEND, WRITE_ATOMIC };

// Writes the len bytes of text to path. It returns 0, or -1 with errno set.
int write_text(const char *path, const char *text, size_t len, enum write_mode mode);

#endif
