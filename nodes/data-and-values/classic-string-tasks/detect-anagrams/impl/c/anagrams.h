// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

#ifndef ANAGRAMS_H
#define ANAGRAMS_H

#include <stdbool.h>

// Returns true if a and b hold each letter from a to z the same number of
// times. Case, spaces, punctuation and digits do not count.
bool are_anagrams(const char *a, const char *b);

#endif
