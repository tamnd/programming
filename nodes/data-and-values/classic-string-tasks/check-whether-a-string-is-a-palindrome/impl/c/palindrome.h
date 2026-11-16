// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

#ifndef PALINDROME_H
#define PALINDROME_H

#include <stdbool.h>

// Returns true if the ASCII letters and digits of text, in lower case, read
// the same backwards. Other characters do not count.
bool is_palindrome(const char *text);

#endif
