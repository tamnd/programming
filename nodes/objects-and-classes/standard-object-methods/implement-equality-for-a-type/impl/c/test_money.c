// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

#include "money.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *name, const int64_t *amounts, const char *const *currencies, int n, int want) {
    int got = count_distinct(amounts, currencies, n);
    if (got != want) {
        fprintf(stderr, "case %s failed: got %d, want %d\n", name, got, want);
        failures++;
    }
}

static void expect(const char *name, bool ok) {
    if (!ok) {
        fprintf(stderr, "check %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("two-equal", (int64_t[]){1250, 1250}, (const char *[]){"EUR", "EUR"}, 2, 1);
    check("different-amounts", (int64_t[]){100, 200, 100}, (const char *[]){"USD", "USD", "USD"}, 3, 2);
    check("mixed", (int64_t[]){500, 500, 500, 700}, (const char *[]){"EUR", "USD", "EUR", "EUR"}, 4, 3);
    check("empty", NULL, NULL, 0, 0);
    check("case", (int64_t[]){999, 999, 999}, (const char *[]){"gbp", "GBP", "Gbp"}, 3, 1);
    check("other-currency", (int64_t[]){0, 0, 0}, (const char *[]){"EUR", "USD", "JPY"}, 3, 3);
    check("negative", (int64_t[]){-5, 5, -5}, (const char *[]){"EUR", "EUR", "EUR"}, 3, 2);
    check("limit", (int64_t[]){1000000000000, -1000000000000, 1000000000000}, (const char *[]){"usd", "USD", "USD"}, 3, 2);

    Money a = money_make(1250, "EUR"), b = money_make(1250, "eur");
    expect("reflexive", money_equal(&a, &a));
    expect("symmetric", money_equal(&a, &b) && money_equal(&b, &a));
    expect("hash", money_hash(&a) == money_hash(&b));
    Money c = money_make(1251, "EUR"), d = money_make(1250, "USD");
    expect("other amount", !money_equal(&a, &c));
    expect("other currency", !money_equal(&a, &d));

    if (failures == 0) {
        printf("all cases passed\n");
    }
    return failures == 0 ? 0 : 1;
}
