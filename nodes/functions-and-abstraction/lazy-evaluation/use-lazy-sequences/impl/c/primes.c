// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

#include "primes.h"

bool is_prime(int k) {
    if (k < 2) {
        return false;
    }
    for (int d = 2; d * d <= k; d++) {
        if (k % d == 0) {
            return false;
        }
    }
    return true;
}

struct primes primes_start(void) {
    return (struct primes){2};
}

int primes_next(struct primes *seq) {
    // Test the numbers from seq->next until one is prime; keep the place.
    while (!is_prime(seq->next)) {
        seq->next++;
    }
    return seq->next++;
}

size_t first_primes(size_t n, int *out) {
    struct primes seq = primes_start();
    for (size_t i = 0; i < n; i++) {
        out[i] = primes_next(&seq);
    }
    return n;
}
