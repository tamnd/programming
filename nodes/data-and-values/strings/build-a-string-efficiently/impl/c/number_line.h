// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

#ifndef NUMBER_LINE_H
#define NUMBER_LINE_H

// number_line returns the numbers 1 to n, separated by commas, in a new
// string that the caller frees. It returns NULL if an allocation fails.
char *number_line(int n);

#endif
