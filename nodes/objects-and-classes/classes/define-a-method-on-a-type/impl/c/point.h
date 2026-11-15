// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

#ifndef POINT_H
#define POINT_H

// C has no methods. A function whose first parameter is the struct plays the
// part of a method, and the name prefix point_ groups the functions.
struct point {
    int x;
    int y;
};

// point_translate returns p moved by (dx, dy). p is passed by value.
struct point point_translate(struct point p, int dx, int dy);

// point_manhattan returns the grid distance |dx| + |dy| between p and q.
int point_manhattan(struct point p, struct point q);

// point_report writes [q.x, q.y, manhattan(p, q), manhattan(q, origin)] to
// out[0] to out[3], where p = (x, y) and q = point_translate(p, dx, dy).
void point_report(int x, int y, int dx, int dy, int out[4]);

#endif
