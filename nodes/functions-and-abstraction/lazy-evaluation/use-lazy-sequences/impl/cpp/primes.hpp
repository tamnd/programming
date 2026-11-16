// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

#pragma once

#include <cstddef>
#include <vector>

// is_prime reports whether k is a prime number, by trial division.
bool is_prime(int k);

// first_primes returns the first n prime numbers.
std::vector<int> first_primes(std::size_t n);
