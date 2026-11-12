// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.
#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <stdlib.h>

// characters returns a new array of *n new strings, one for each code point of
// the UTF-8 string s, or NULL if there is no memory. Free it with
// free_characters.
char **characters(const char *s, size_t *n);

void free_characters(char **chars, size_t n);

#endif
