// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

#ifndef COMPARE_SETS_H
#define COMPARE_SETS_H

#include <stdbool.h>
#include <stddef.h>

// set_ops has the results of the set operations. The caller gives the four
// arrays, each with room for na + nb pointers. compare_sets sets the counts.
struct set_ops {
    const char **either;      // the union
    size_t n_either;
    const char **both;        // the intersection
    size_t n_both;
    const char **only_a;      // the difference a minus b
    size_t n_only_a;
    const char **exactly_one; // the symmetric difference
    size_t n_exactly_one;
    bool a_in_b;              // the set of a is a subset of the set of b
};

// compare_sets sorts a and b in place by strcmp, then does the set operations
// on their words. Each result array is in ascending order with no repeats. The
// pointers in the results point to the strings of a and b.
void compare_sets(const char **a, size_t na, const char **b, size_t nb, struct set_ops *r);

#endif
