// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

#ifndef TOGGLE_H
#define TOGGLE_H

#include <stddef.h>

// toggle adds each of the n words that is not in the set, and removes each
// word that is in it. The set is the array out, in ascending order by
// strcmp. out must have room for n pointers. It returns the size of the set.
// The pointers in out point to the strings of words.
size_t toggle(const char *const *words, size_t n, const char **out);

#endif
