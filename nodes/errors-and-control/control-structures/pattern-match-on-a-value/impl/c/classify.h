// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

#ifndef CLASSIFY_H
#define CLASSIFY_H

// classify names the place of the point (x, y): origin, x-axis, y-axis,
// diagonal or other. It returns a string literal; the caller must not free it.
const char *classify(int x, int y);

#endif
