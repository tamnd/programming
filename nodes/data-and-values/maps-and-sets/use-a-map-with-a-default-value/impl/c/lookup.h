// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

#ifndef LOOKUP_H
#define LOOKUP_H

#include <stddef.h>

// An entry of a map. A map is an array of entries in ascending order of the
// key by strcmp, with no key two times.
struct entry {
    const char *key;
    int value;
};

// lookup writes to out[i] the value of keys[i] in table (n entries), or
// fallback if table has no entry for keys[i]. out must have room for m ints.
void lookup(const struct entry *table, size_t n, const char *const *keys, size_t m, int fallback, int *out);

#endif
