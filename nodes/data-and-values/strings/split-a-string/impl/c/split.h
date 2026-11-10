// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

#ifndef SPLIT_H
#define SPLIT_H

#include <stddef.h>

// A field is a part of the input string. It is not a copy, and it has no
// terminating null character.
struct field {
    const char *start;
    size_t len;
};

// split splits s on the literal separator sep, which must not be empty, and
// keeps the empty fields. With limit n > 0 there are at most n fields, and the
// last field holds the rest of s. A limit of 0 means no limit. It stores the
// number of fields in *count and returns an array that the caller frees with
// free, or NULL if there is no memory.
struct field *split(const char *s, const char *sep, size_t limit, size_t *count);

// split_whitespace splits s on runs of space, tab and LF. There are no empty
// fields, so a string of only white space gives a count of 0.
struct field *split_whitespace(const char *s, size_t *count);

// split_regex splits s on each match of the POSIX extended regular expression
// pattern, which must not match the empty string. It returns NULL if the
// pattern is not valid or if there is no memory.
struct field *split_regex(const char *s, const char *pattern, size_t *count);

#endif
