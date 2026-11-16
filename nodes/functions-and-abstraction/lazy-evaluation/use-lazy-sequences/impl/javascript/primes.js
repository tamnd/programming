// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

/** Returns true if k is a prime number, by trial division. */
export function isPrime(k) {
  if (k < 2) {
    return false;
  }
  for (let d = 2; d * d <= k; d++) {
    if (k % d === 0) {
      return false;
    }
  }
  return true;
}

/** The infinite sequence start, start + 1, ... It runs only when it is read. */
function* naturals(start) {
  for (let i = start; ; i++) {
    yield i;
  }
}

/** Returns the infinite lazy sequence of the prime numbers. */
export function primes() {
  // Iterator helpers are lazy: filter tests a number only when it is read.
  return naturals(2).filter(isPrime);
}

/**
 * Returns the first n prime numbers.
 * @param {number} n from 0
 * @returns {number[]}
 */
export function firstPrimes(n) {
  return primes().take(n).toArray();
}
