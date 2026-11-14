// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

// run_ops runs the n operations on a new linked list and writes its items,
// from front to back, to out. out must have room for n items. It returns the
// number of items, or -1 if an operation is not valid or memory runs out.
ptrdiff_t run_ops(const char *const *ops, size_t n, int *out);

#endif
