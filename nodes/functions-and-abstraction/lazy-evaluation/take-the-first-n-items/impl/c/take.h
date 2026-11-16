// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

#ifndef TAKE_H
#define TAKE_H

#include <stddef.h>

// take copies the first n items of items[0..len) to out and returns their
// number, which is the smaller of n and len. out must have room for that
// many items.
size_t take(const int *items, size_t len, size_t n, int *out);

// struct naturals is a lazy infinite sequence: it holds only the next value.
struct naturals {
    int next;
};

// naturals_next returns the next value of the sequence and moves it on.
int naturals_next(struct naturals *seq);

// take_naturals writes the first n items of the sequence start, start + 1, ...
// to out and returns n. out must have room for n items.
size_t take_naturals(int start, size_t n, int *out);

#endif
