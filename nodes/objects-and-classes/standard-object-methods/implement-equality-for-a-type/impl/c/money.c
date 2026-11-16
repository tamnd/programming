// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

#include "money.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Money money_make(int64_t cents, const char *currency) {
    Money m;
    memset(&m, 0, sizeof m);
    m.cents = cents;
    for (int i = 0; i < 3 && currency[i] != '\0'; i++) {
        m.currency[i] = (char)toupper((unsigned char)currency[i]); // "eur" and "EUR" are the same currency
    }
    return m;
}

// Compare the fields, not the bytes: memcmp would also compare the padding of the struct.
bool money_equal(const Money *a, const Money *b) {
    return a->cents == b->cents && strcmp(a->currency, b->currency) == 0;
}

// FNV-1a over the same fields as money_equal, so equal values have equal hashes.
uint64_t money_hash(const Money *m) {
    uint64_t h = 14695981039346656037u;
    uint64_t c = (uint64_t)m->cents;
    for (int i = 0; i < 8; i++) {
        h = (h ^ ((c >> (8 * i)) & 0xff)) * 1099511628211u;
    }
    for (const char *p = m->currency; *p != '\0'; p++) {
        h = (h ^ (unsigned char)*p) * 1099511628211u;
    }
    return h;
}

// A hash set with open addressing. Returns -1 if memory runs out.
int count_distinct(const int64_t *amounts, const char *const *currencies, int n) {
    size_t size = 16;
    while (size < 2 * (size_t)n) {
        size *= 2;
    }
    Money *slots = malloc(size * sizeof *slots);
    bool *used = calloc(size, sizeof *used);
    if (slots == NULL || used == NULL) {
        free(slots);
        free(used);
        return -1;
    }
    int count = 0;
    for (int i = 0; i < n; i++) {
        Money m = money_make(amounts[i], currencies[i]);
        size_t j = (size_t)(money_hash(&m) & (size - 1));
        while (used[j] && !money_equal(&slots[j], &m)) {
            j = (j + 1) & (size - 1);
        }
        if (!used[j]) {
            used[j] = true;
            slots[j] = m;
            count++;
        }
    }
    free(slots);
    free(used);
    return count;
}
