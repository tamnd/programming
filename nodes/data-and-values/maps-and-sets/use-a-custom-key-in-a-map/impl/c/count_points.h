// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

#ifndef COUNT_POINTS_H
#define COUNT_POINTS_H

#include <stddef.h>

struct point {
    int x;
    int y;
};

struct point_count {
    struct point p;
    size_t n;
};

// count_points sorts the n points in place in order of x, then y. Then it
// writes one entry for each distinct point to out, in that order. out must
// have room for n entries. It returns the number of entries.
size_t count_points(struct point *points, size_t n, struct point_count *out);

#endif
