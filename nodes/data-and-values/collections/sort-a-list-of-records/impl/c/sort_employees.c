// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

#include "sort_employees.h"

#include <stdlib.h>
#include <string.h>

// qsort is not stable, so each record keeps its input position as the last key.
struct ranked {
    struct employee e;
    size_t pos;
};

static int compare(const void *pa, const void *pb) {
    const struct ranked *a = pa, *b = pb;
    // strcmp compares bytes as unsigned char, so UTF-8 text sorts by code point.
    int c = strcmp(a->e.dept, b->e.dept);
    if (c != 0) {
        return c;
    }
    // Not b - a: that can overflow. Here b comes first, so high salaries come first.
    if (a->e.salary != b->e.salary) {
        return (b->e.salary > a->e.salary) - (b->e.salary < a->e.salary);
    }
    return (a->pos > b->pos) - (a->pos < b->pos);
}

int sort_employees(struct employee *employees, size_t n) {
    if (n < 2) {
        return 0;
    }
    struct ranked *r = malloc(n * sizeof *r);
    if (r == NULL) {
        return -1;
    }
    for (size_t i = 0; i < n; i++) {
        r[i] = (struct ranked){employees[i], i};
    }
    qsort(r, n, sizeof *r, compare);
    for (size_t i = 0; i < n; i++) {
        employees[i] = r[i].e;
    }
    free(r);
    return 0;
}
