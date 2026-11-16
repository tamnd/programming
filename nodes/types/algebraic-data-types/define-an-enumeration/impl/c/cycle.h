// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

#ifndef CYCLE_H
#define CYCLE_H

#include <stddef.h>

// The days of the week. The enumeration constants are the ints 0 to 6, and
// DAY_COUNT is 7.
enum day { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY, DAY_COUNT };

// day_name gives the name of d in lower case, or NULL if d is not a day.
const char *day_name(enum day d);

// cycle writes steps names to out, from the day with the name start on, with
// monday after sunday. out must have room for steps names. It returns the
// number of names: steps, or 0 if start is not a name.
size_t cycle(const char *start, size_t steps, const char **out);

#endif
