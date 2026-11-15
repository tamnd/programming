// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

#ifndef COUNT_TO_H
#define COUNT_TO_H

// count_to returns a new array of the n numbers 1, 2, ..., n. The caller must
// free it. It returns NULL when malloc fails.
int *count_to(int n);

#endif
