// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

#include "point.h"

#include <stdlib.h>

struct point point_translate(struct point p, int dx, int dy) {
    return (struct point){p.x + dx, p.y + dy};
}

int point_manhattan(struct point p, struct point q) {
    return abs(p.x - q.x) + abs(p.y - q.y);
}

void point_report(int x, int y, int dx, int dy, int out[4]) {
    const struct point origin = {0, 0};
    struct point p = {x, y};
    struct point q = point_translate(p, dx, dy);
    out[0] = q.x;
    out[1] = q.y;
    out[2] = point_manhattan(p, q);
    out[3] = point_manhattan(q, origin);
}
