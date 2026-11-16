// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

#include "compose.h"

#include <string.h>

static int inc(int x) { return x + 1; }
static int dec(int x) { return x - 1; }
static int twice(int x) { return 2 * x; }
static int negate(int x) { return -x; }

int apply_composed(const int_fn fs[], size_t n, int x) {
    for (size_t i = n; i > 0; i--) {
        x = fs[i - 1](x);
    }
    return x;
}

int_fn op_by_name(const char *name) {
    if (strcmp(name, "inc") == 0) {
        return inc;
    }
    if (strcmp(name, "dec") == 0) {
        return dec;
    }
    if (strcmp(name, "double") == 0) {
        return twice;
    }
    if (strcmp(name, "negate") == 0) {
        return negate;
    }
    return NULL;
}

int compose_all(const char *const ops[], size_t n, int x) {
    int_fn fs[COMPOSE_MAX];
    for (size_t i = 0; i < n; i++) {
        fs[i] = op_by_name(ops[i]);
    }
    return apply_composed(fs, n, x);
}
