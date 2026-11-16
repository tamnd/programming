// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

#include "vec2.h"

Vec2 vec2_add(Vec2 a, Vec2 b) { return (Vec2){a.x + b.x, a.y + b.y}; }

bool vec2_equal(Vec2 a, Vec2 b) { return a.x == b.x && a.y == b.y; }

Vec2 vector_sum(const long long *xs, const long long *ys, size_t n) {
    Vec2 total = {0, 0};
    for (size_t i = 0; i < n; i++) {
        total = vec2_add(total, (Vec2){xs[i], ys[i]});
    }
    return total;
}
