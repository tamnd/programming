// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

#ifndef RECTANGLE_H
#define RECTANGLE_H

// C has no classes. A struct holds the fields, and functions that take a
// pointer to the struct play the part of methods.
struct rectangle {
    int width;
    int height;
};

struct rectangle rectangle_make(int width, int height);
int rectangle_area(const struct rectangle *r);
int rectangle_perimeter(const struct rectangle *r);
void rectangle_scale(struct rectangle *r, int factor);

// rectangle_report writes the area and the perimeter, before and after
// rectangle_scale(factor), to out[0] to out[3].
void rectangle_report(int width, int height, int factor, int out[4]);

#endif
