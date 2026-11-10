// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

#ifndef REFORMAT_DATES_H
#define REFORMAT_DATES_H

#include <stddef.h>

struct date {
    char year[5];
    char month[3];
    char day[3];
};

// Returns a new string with each date YYYY<from_sep>MM<from_sep>DD in text changed to DD<to_sep>MM<to_sep>YYYY,
// or NULL if the pattern cannot be compiled or memory runs out. The caller frees the string.
char *reformat_dates(const char *text, const char *from_sep, const char *to_sep);

// Stores a new array with each date YYYY<sep>MM<sep>DD in text in *out, and its length in *n.
// Returns 0, or -1 on an error. The caller frees *out.
int find_dates(const char *text, const char *sep, struct date **out, size_t *n);

#endif
