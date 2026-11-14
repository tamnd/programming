// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

#include "count_points.h"

#include <stdbool.h>
#include <stdlib.h>

// cmp orders points by x, then y. It compares the values, because a - b can
// overflow an int.
static int cmp(const void *a, const void *b) {
    const struct point *p = a;
    const struct point *q = b;
    if (p->x != q->x) {
        return (p->x > q->x) - (p->x < q->x);
    }
    return (p->y > q->y) - (p->y < q->y);
}

// equal compares the fields. C has no == for structs, and memcmp would also
// compare the padding bytes of a struct that has them.
static bool equal(struct point a, struct point b) {
    return a.x == b.x && a.y == b.y;
}

size_t count_points(struct point *points, size_t n, struct point_count *out) {
    // C has no map type. After a sort, equal points are next to each other,
    // so one pass counts them.
    if (n > 0) {
        qsort(points, n, sizeof points[0], cmp);
    }
    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        if (k > 0 && equal(out[k - 1].p, points[i])) {
            out[k - 1].n++;
        } else {
            out[k++] = (struct point_count){points[i], 1};
        }
    }
    return k;
}
