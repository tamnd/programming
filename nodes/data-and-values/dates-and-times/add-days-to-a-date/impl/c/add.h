// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

#ifndef ADD_TO_DATE_H
#define ADD_TO_DATE_H

#include <stddef.h>

// Adds years and months at once, then days, and writes the date as
// YYYY-MM-DD to out. A day after the end of the new month becomes its last
// day. Returns 0, or -1 if out is too small.
int add_to_date(int year, int month, int day, int years, int months, int days, char *out, size_t size);

#endif
