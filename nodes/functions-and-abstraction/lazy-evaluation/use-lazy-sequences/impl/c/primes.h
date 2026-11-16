// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

#ifndef PRIMES_H
#define PRIMES_H

#include <stdbool.h>
#include <stddef.h>

// is_prime reports whether k is a prime number, by trial division.
bool is_prime(int k);

// struct primes is a lazy infinite sequence of the prime numbers. It holds
// only the next number to test.
struct primes {
    int next;
};

// primes_start returns a sequence that begins at 2.
struct primes primes_start(void);

// primes_next returns the next prime number of the sequence.
int primes_next(struct primes *seq);

// first_primes writes the first n prime numbers to out and returns n. out
// must have room for n numbers.
size_t first_primes(size_t n, int *out);

#endif
