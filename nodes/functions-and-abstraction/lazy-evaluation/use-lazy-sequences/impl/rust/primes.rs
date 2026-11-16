// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

/// Returns true if k is a prime number, by trial division.
pub fn is_prime(k: u32) -> bool {
    if k < 2 {
        return false;
    }
    let mut d = 2;
    while d * d <= k {
        if k % d == 0 {
            return false;
        }
        d += 1;
    }
    true
}

/// Returns the infinite lazy sequence of the prime numbers.
pub fn primes() -> impl Iterator<Item = u32> {
    // 2.. has no end, and filter is lazy: it runs only for the numbers that are read.
    (2..).filter(|&k| is_prime(k))
}

/// Returns the first n prime numbers.
pub fn first_primes(n: usize) -> Vec<u32> {
    primes().take(n).collect()
}
