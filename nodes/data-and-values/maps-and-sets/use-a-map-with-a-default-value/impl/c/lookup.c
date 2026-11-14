// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

#include "lookup.h"

#include <stdlib.h>
#include <string.h>

// cmp compares a key (a string) with the key of an entry.
static int cmp(const void *key, const void *elem) {
    const struct entry *e = elem;
    return strcmp(key, e->key);
}

void lookup(const struct entry *table, size_t n, const char *const *keys, size_t m, int fallback, int *out) {
    for (size_t i = 0; i < m; i++) {
        // bsearch returns a null pointer for a missing key, so a stored 0 and
        // a missing key are different.
        const struct entry *e = n == 0 ? NULL : bsearch(keys[i], table, n, sizeof table[0], cmp);
        out[i] = e == NULL ? fallback : e->value;
    }
}
