// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

#ifndef BIG_POWER_H
#define BIG_POWER_H

// Returns base to the power exp as decimal text in a new string, or NULL if
// memory runs out. The caller frees the string. -1000 <= base <= 1000.
char *big_power(int base, int exp);

#endif
