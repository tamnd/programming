// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

#include "primes.h"

#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list has a 0 in it, because C has no empty array.
#define CHECK(name, n, expected)                                               \
    do {                                                                       \
        const int w[] = expected;                                              \
        int out[64];                                                           \
        size_t k = first_primes(n, out);                                       \
        int ok = k == (size_t)(n);                                             \
        for (size_t i = 0; ok && i < k; i++) {                                 \
            ok = out[i] == w[i];                                               \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("five", 5, L(2, 3, 5, 7, 11));
    CHECK("ten", 10, L(2, 3, 5, 7, 11, 13, 17, 19, 23, 29));
    CHECK("twenty-five", 25, L(2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97));
    CHECK("zero", 0, L(0));
    CHECK("one", 1, L(2));
    CHECK("two", 2, L(2, 3));
    CHECK("past-a-gap", 31, L(2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127));
    // The sequence keeps its place between calls.
    struct primes seq = primes_start();
    primes_next(&seq);
    primes_next(&seq);
    if (primes_next(&seq) != 5 || seq.next != 6) {
        fprintf(stderr, "case lazy failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
