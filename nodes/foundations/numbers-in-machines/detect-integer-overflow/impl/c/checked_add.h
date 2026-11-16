// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

#ifndef CHECKED_ADD_H
#define CHECKED_ADD_H

#include <stdbool.h>
#include <stdint.h>

// checked_add stores a + b in *out and returns true, or returns false when the
// sum does not fit in int64_t. It uses the GCC and Clang builtin.
bool checked_add(int64_t a, int64_t b, int64_t *out);

// checked_add_portable does the same in standard C, with a test before the
// addition.
bool checked_add_portable(int64_t a, int64_t b, int64_t *out);

#endif
