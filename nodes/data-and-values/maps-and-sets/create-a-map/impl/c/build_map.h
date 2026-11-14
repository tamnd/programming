// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

#ifndef BUILD_MAP_H
#define BUILD_MAP_H

#include <stddef.h>

// An entry of the map. The key points into the caller's strings.
struct entry {
    const char *key;
    int value;
};

// build_map puts the n pairs in a map, removes the m keys in drop, and
// writes the entries to out in ascending order of the key. out must have room
// for n entries. It returns the number of entries.
size_t build_map(const struct entry *pairs, size_t n, const char *const *drop, size_t m, struct entry *out);

#endif
