// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

#ifndef SUBSTRING_H
#define SUBSTRING_H

#include <stddef.h>

// substring returns the code points of the UTF-8 string s from index start up
// to, not including, end, in a new string that the caller frees. It returns
// NULL if the allocation fails.
char *substring(const char *s, size_t start, size_t end);

#endif
