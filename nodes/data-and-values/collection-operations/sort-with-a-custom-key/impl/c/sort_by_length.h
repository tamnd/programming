// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

#ifndef SORT_BY_LENGTH_H
#define SORT_BY_LENGTH_H

#include <stddef.h>

// sort_by_length sorts the n words in place: in order of the length, then of
// the bytes of the word.
void sort_by_length(const char **words, size_t n);

#endif
