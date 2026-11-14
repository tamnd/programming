// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

#ifndef SCORE_LINES_H
#define SCORE_LINES_H

#include <stddef.h>

// An entry of the map. The keys of a map are all different.
struct entry {
    const char *key;
    int value;
};

// score_lines writes one line "key=value\n" for each of the n entries, in
// ascending order of the key, then the line "total=N\n" with the sum of the
// values, to buf. sorted must have room for n pointers. It returns the length
// of the text, or -1 if the text and its terminating null do not fit in cap.
int score_lines(const struct entry *map, size_t n, const struct entry **sorted, char *buf, size_t cap);

#endif
