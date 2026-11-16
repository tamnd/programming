// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

#ifndef WRAP_H
#define WRAP_H

#include <stddef.h>

// Writes the lines of text into out, each followed by a newline, with the
// greedy method: each line is at most width characters long, and a word
// longer than width is a line of its own. Returns the count of lines, or -1
// if out is too small. out must hold strlen(text) + 2 bytes.
int wrap(const char *text, size_t width, char *out, size_t size);

#endif
