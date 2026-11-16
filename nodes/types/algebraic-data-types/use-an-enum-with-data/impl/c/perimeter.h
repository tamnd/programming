// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

#ifndef PERIMETER_H
#define PERIMETER_H

#include <stdbool.h>
#include <stddef.h>

// C has no enum with data. A tagged union pairs an enum, the tag, with a
// union that holds the data of the variant. Read only the union member that
// the tag names.
enum shape_kind { SHAPE_SQUARE, SHAPE_RECTANGLE, SHAPE_TRIANGLE };

struct shape {
    enum shape_kind kind;
    union {
        struct {
            int side;
        } square;
        struct {
            int width, height;
        } rectangle;
        struct {
            int a, b, c;
        } triangle;
    } as;
};

// make_shape fills *out and returns true, or returns false when the data is
// not a valid shape.
bool make_shape(const char *kind, const int *sides, size_t n, struct shape *out);

int shape_perimeter(const struct shape *s);

// perimeter stores the perimeter in *out and returns true, or returns false
// when the data is not a valid shape.
bool perimeter(const char *kind, const int *sides, size_t n, int *out);

#endif
