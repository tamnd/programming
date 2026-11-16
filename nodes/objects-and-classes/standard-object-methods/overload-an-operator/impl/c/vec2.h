// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

#ifndef VEC2_H
#define VEC2_H

#include <stdbool.h>
#include <stddef.h>

// C has no user operators, so the type has functions in place of + and ==.
typedef struct {
    long long x, y;
} Vec2;

Vec2 vec2_add(Vec2 a, Vec2 b);
bool vec2_equal(Vec2 a, Vec2 b);

// vector_sum returns the sum of the vectors (xs[i], ys[i]).
Vec2 vector_sum(const long long *xs, const long long *ys, size_t n);

#endif
