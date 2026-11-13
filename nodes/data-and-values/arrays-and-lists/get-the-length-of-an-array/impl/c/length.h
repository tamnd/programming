// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

#ifndef LENGTH_H
#define LENGTH_H

#include <stddef.h>

// length returns the number of strings in items, which ends with a null
// pointer, like argv. A function gets a pointer, so sizeof cannot give the
// length; the null pointer marks the end instead.
size_t length(const char *const *items);

#endif
