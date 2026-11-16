// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

#ifndef MONEY_H
#define MONEY_H

#include <stdbool.h>
#include <stdint.h>

// An amount in cents and an upper-case 3-letter currency code.
typedef struct {
    int64_t cents;
    char currency[4];
} Money;

Money money_make(int64_t cents, const char *currency);
bool money_equal(const Money *a, const Money *b);
uint64_t money_hash(const Money *m);
int count_distinct(const int64_t *amounts, const char *const *currencies, int n);

#endif
