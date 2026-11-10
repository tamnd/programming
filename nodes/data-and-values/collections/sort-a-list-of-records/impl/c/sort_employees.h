// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

#ifndef SORT_EMPLOYEES_H
#define SORT_EMPLOYEES_H

#include <stddef.h>

struct employee {
    const char *name;
    const char *dept;  // UTF-8
    long salary;       // long has at least 32 bits
};

// sort_employees sorts the n records in place by dept, then by salary from
// high to low. Records with equal keys keep their input order. It returns 0,
// or -1 if it cannot get memory; then the records do not change.
int sort_employees(struct employee *employees, size_t n);

#endif
