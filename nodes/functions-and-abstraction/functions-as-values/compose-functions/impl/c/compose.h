// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

#ifndef COMPOSE_H
#define COMPOSE_H

#include <stddef.h>

// A pointer to a function from one int to one int.
typedef int (*int_fn)(int);

// The largest number of names that compose_all takes.
#define COMPOSE_MAX 20

// Applies the n functions in fs to x, from the last to the first. C cannot
// make a new function at run time, so the helper takes the value too.
int apply_composed(const int_fn fs[], size_t n, int x);

// Returns the function that name names, or NULL for an unknown name.
int_fn op_by_name(const char *name);

// Composes the functions that the n names in ops name and applies the
// result to x. n is at most COMPOSE_MAX.
int compose_all(const char *const ops[], size_t n, int x);

#endif
