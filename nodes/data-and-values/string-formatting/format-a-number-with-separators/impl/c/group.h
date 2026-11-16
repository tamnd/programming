// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

#ifndef GROUP_H
#define GROUP_H

// The size of a buffer that holds each result, with the null byte.
#define GROUP_THOUSANDS_SIZE 27

// Writes n in decimal with a comma between each group of 3 digits into out,
// which must hold GROUP_THOUSANDS_SIZE bytes.
void group_thousands(char *out, long long n);

#endif
