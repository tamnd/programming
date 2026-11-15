// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

#include "point.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int x, int y, int dx, int dy, int a, int b, int c, int d) {
    const int want[4] = {a, b, c, d};
    int got[4];
    point_report(x, y, dx, dy, got);
    if (memcmp(got, want, sizeof want) != 0) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("right-and-up", 1, 2, 3, 4, 4, 6, 7, 10);
    check("mixed-signs", 5, -1, -2, 3, 3, 2, 5, 5);
    check("zero-move", 7, -3, 0, 0, 7, -3, 0, 10);
    check("negative", -2, -5, -3, -1, -5, -6, 4, 11);
    check("origin", 2, 3, -2, -3, 0, 0, 5, 0);
    check("limit", 100000000, -100000000, 100000000, -100000000, 200000000, -200000000, 200000000, 400000000);

    // point_translate gets a copy, so p does not change.
    struct point p = {1, 2};
    struct point q = point_translate(p, 3, 4);
    if (p.x != 1 || p.y != 2 || q.x != 4 || q.y != 6) {
        fprintf(stderr, "case another-point failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
