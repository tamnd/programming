// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

#include "big_power.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LIMB 1000000000u // each limb holds 9 decimal digits

char *big_power(int base, int exp) {
    uint32_t m = base < 0 ? (uint32_t)(-(int64_t)base) : (uint32_t)base;
    size_t cap = (size_t)exp + 1; // each step adds at most one limb
    size_t n = 1;
    uint32_t *limbs = malloc(cap * sizeof *limbs); // least significant first
    char *out = malloc(2 + 9 * cap);
    if (limbs == NULL || out == NULL) {
        free(limbs);
        free(out);
        return NULL;
    }
    limbs[0] = 1;
    for (int i = 0; i < exp; i++) {
        uint64_t carry = 0;
        for (size_t j = 0; j < n; j++) {
            uint64_t t = (uint64_t)limbs[j] * m + carry;
            limbs[j] = (uint32_t)(t % LIMB);
            carry = t / LIMB;
        }
        if (carry != 0) {
            limbs[n++] = (uint32_t)carry;
        }
    }
    char *p = out;
    if (base < 0 && exp % 2 == 1) {
        *p++ = '-';
    }
    p += sprintf(p, "%" PRIu32, limbs[n - 1]);
    for (size_t j = n - 1; j-- > 0;) {
        p += sprintf(p, "%09" PRIu32, limbs[j]); // keep the inner zeros
    }
    free(limbs);
    return out;
}
