// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

#ifndef IS_CLOSE_H
#define IS_CLOSE_H

#include <stdbool.h>

// Tells whether a and b are close: |a - b| <= max(rel_tol * max(|a|, |b|), abs_tol).
// NaN is close to no value, and an infinity is close only to itself.
bool is_close(double a, double b, double rel_tol, double abs_tol);

#endif
