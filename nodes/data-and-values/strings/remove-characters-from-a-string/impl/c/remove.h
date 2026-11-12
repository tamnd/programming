// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

#ifndef REMOVE_H
#define REMOVE_H

// remove_chars returns a new string with each code point of chars removed from
// s, or NULL if there is no memory. s and chars are valid UTF-8. The caller
// frees the result.
char *remove_chars(const char *s, const char *chars);

#endif
