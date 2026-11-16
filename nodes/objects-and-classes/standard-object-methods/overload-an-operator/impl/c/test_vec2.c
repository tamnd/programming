// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

#include "vec2.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *name, const long long *xs, const long long *ys, size_t n, long long wx, long long wy) {
    Vec2 got = vector_sum(xs, ys, n);
    if (got.x != wx || got.y != wy) {
        fprintf(stderr, "case %s failed: got (%lld, %lld), want (%lld, %lld)\n", name, got.x, got.y, wx, wy);
        failures++;
    }
}

static void expect(const char *name, bool ok) {
    if (!ok) {
        fprintf(stderr, "check %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("two", (long long[]){1, 3}, (long long[]){2, 4}, 2, 4, 6);
    check("three", (long long[]){1, 2, 3}, (long long[]){10, 20, 30}, 3, 6, 60);
    check("empty", NULL, NULL, 0, 0, 0);
    check("single", (long long[]){5}, (long long[]){-7}, 1, 5, -7);
    check("negative", (long long[]){-1, -2}, (long long[]){-3, -4}, 2, -3, -7);
    check("cancel", (long long[]){3, -3}, (long long[]){5, -5}, 2, 0, 0);
    check("limit", (long long[]){1000000000, 1000000000, 1000000000},
          (long long[]){-1000000000, -1000000000, -1000000000}, 3, 3000000000LL, -3000000000LL);

    Vec2 a = {1, 2}, b = {3, 4};
    expect("add", vec2_equal(vec2_add(a, b), (Vec2){4, 6}));
    expect("equal", vec2_equal(a, (Vec2){1, 2}) && !vec2_equal(a, b));

    if (failures == 0) {
        printf("all tests passed\n");
    }
    return failures == 0 ? 0 : 1;
}
