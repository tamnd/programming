// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

#include "perimeter.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *label, const char *kind, const int *sides, size_t n, bool want_ok, int want) {
    int got = 0;
    bool ok = perimeter(kind, sides, n, &got);
    if (ok != want_ok || (ok && got != want)) {
        fprintf(stderr, "case %s failed\n", label);
        failures++;
    }
}

// The last element of each list is a dummy, so that an empty list is valid C.
#define L(...) ((const int[]){__VA_ARGS__ 0})
#define N(...) (sizeof L(__VA_ARGS__) / sizeof(int) - 1)
#define CHECK(label, kind, list, ok, want) check(label, kind, L list, N list, ok, want)

int main(void) {
    CHECK("square", "square", (3,), true, 12);
    CHECK("rectangle", "rectangle", (2, 5,), true, 14);
    CHECK("triangle", "triangle", (3, 4, 5,), true, 12);
    CHECK("unknown-kind", "circle", (2,), false, 0);
    CHECK("wrong-case", "Square", (3,), false, 0);
    CHECK("too-few-sides", "rectangle", (4,), false, 0);
    CHECK("too-many-sides", "square", (3, 3,), false, 0);
    CHECK("no-sides", "triangle", (), false, 0);
    CHECK("zero-side", "square", (0,), false, 0);
    CHECK("negative-side", "rectangle", (-2, 5,), false, 0);
    CHECK("not-a-triangle", "triangle", (1, 2, 3,), false, 0);
    CHECK("largest", "triangle", (1000000, 1000000, 1000000,), true, 3000000);

    struct shape s;
    if (!make_shape("rectangle", (const int[]){2, 5}, 2, &s) || s.kind != SHAPE_RECTANGLE ||
        s.as.rectangle.width != 2 || s.as.rectangle.height != 5) {
        fprintf(stderr, "case variant-data failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
