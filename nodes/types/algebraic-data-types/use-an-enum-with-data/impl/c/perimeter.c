// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

#include "perimeter.h"

#include <string.h>

bool make_shape(const char *kind, const int *sides, size_t n, struct shape *out) {
    for (size_t i = 0; i < n; i++) {
        if (sides[i] <= 0) {
            return false;
        }
    }
    if (strcmp(kind, "square") == 0 && n == 1) {
        out->kind = SHAPE_SQUARE;
        out->as.square.side = sides[0];
        return true;
    }
    if (strcmp(kind, "rectangle") == 0 && n == 2) {
        out->kind = SHAPE_RECTANGLE;
        out->as.rectangle.width = sides[0];
        out->as.rectangle.height = sides[1];
        return true;
    }
    if (strcmp(kind, "triangle") == 0 && n == 3) {
        int a = sides[0], b = sides[1], c = sides[2];
        if (a < b + c && b < a + c && c < a + b) {
            out->kind = SHAPE_TRIANGLE;
            out->as.triangle.a = a;
            out->as.triangle.b = b;
            out->as.triangle.c = c;
            return true;
        }
    }
    return false;
}

int shape_perimeter(const struct shape *s) {
    switch (s->kind) {
    case SHAPE_SQUARE:
        return 4 * s->as.square.side;
    case SHAPE_RECTANGLE:
        return 2 * (s->as.rectangle.width + s->as.rectangle.height);
    case SHAPE_TRIANGLE:
        return s->as.triangle.a + s->as.triangle.b + s->as.triangle.c;
    }
    return -1; // not reached for a valid tag
}

bool perimeter(const char *kind, const int *sides, size_t n, int *out) {
    struct shape s;
    if (!make_shape(kind, sides, n, &s)) {
        return false;
    }
    *out = shape_perimeter(&s);
    return true;
}
