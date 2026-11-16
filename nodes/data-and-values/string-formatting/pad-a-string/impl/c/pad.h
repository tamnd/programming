// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

#ifndef PAD_H
#define PAD_H

#include <stddef.h>

// Writes s padded with fill to width into left (padding on the left) and
// into right (padding on the right). If s has width bytes or more, both are
// copies of s. Each buffer must hold max(strlen(s), width) + 1 bytes.
void pad(const char *s, size_t width, char fill, char *left, char *right);

#endif
