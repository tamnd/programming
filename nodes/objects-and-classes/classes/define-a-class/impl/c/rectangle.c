// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

#include "rectangle.h"

struct rectangle rectangle_make(int width, int height) {
    return (struct rectangle){.width = width, .height = height};
}

// A const pointer: the function reads the rectangle and cannot change it.
int rectangle_area(const struct rectangle *r) {
    return r->width * r->height;
}

int rectangle_perimeter(const struct rectangle *r) {
    return 2 * (r->width + r->height);
}

void rectangle_scale(struct rectangle *r, int factor) {
    r->width *= factor;
    r->height *= factor;
}

void rectangle_report(int width, int height, int factor, int out[4]) {
    struct rectangle r = rectangle_make(width, height);
    out[0] = rectangle_area(&r);
    out[1] = rectangle_perimeter(&r);
    rectangle_scale(&r, factor);
    out[2] = rectangle_area(&r);
    out[3] = rectangle_perimeter(&r);
}
