// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

#ifndef REPLACE_H
#define REPLACE_H

#include <stddef.h>

// replace returns s with each match of old replaced by new, in a new string
// that the caller frees. old must not be empty. It returns NULL if the
// allocation fails.
char *replace(const char *s, const char *old, const char *new_);

// replace_first is like replace, but it replaces only the first match.
char *replace_first(const char *s, const char *old, const char *new_);

#endif
