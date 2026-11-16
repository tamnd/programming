// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

import java.util.stream.IntStream;

final class Primes {
  private Primes() {}

  /** Returns true if k is a prime number, by trial division. */
  static boolean isPrime(int k) {
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

  /** Returns the infinite lazy stream of the prime numbers. */
  static IntStream primes() {
    // iterate has no end, and filter is lazy: it runs only for the numbers that are read.
    return IntStream.iterate(2, k -> k + 1).filter(Primes::isPrime);
  }

  /** Returns the first n prime numbers. */
  static int[] firstPrimes(int n) {
    // limit is short-circuiting, so the stream stops after n primes.
    return primes().limit(n).toArray();
  }
}
