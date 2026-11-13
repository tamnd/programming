// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

#ifndef REVERSE_LIST_H
#define REVERSE_LIST_H

#include <stddef.h>

// reverse_list returns a new array of the n items in the opposite order. The
// caller must free it. It returns NULL when malloc fails.
int *reverse_list(const int *items, size_t n);

#endif
