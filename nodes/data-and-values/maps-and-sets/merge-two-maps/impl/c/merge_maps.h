// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

#ifndef MERGE_MAPS_H
#define MERGE_MAPS_H

#include <stddef.h>

// An entry of a map. A map is an array of entries in ascending order of the
// key, with no key two times.
struct entry {
    const char *key;
    int value;
};

// merge_maps writes the entries of the maps base (nb entries) and over (no
// entries) to out, in ascending order of the key. For a key in both maps, the
// entry of over wins. out must have room for nb + no entries. It returns the
// number of entries.
size_t merge_maps(const struct entry *base, size_t nb, const struct entry *over, size_t no, struct entry *out);

#endif
