// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

/** Returns true if k is a prime number, by trial division. */
export function isPrime(k: number): boolean {
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
function* naturals(start: number): Generator<number> {
  for (let i = start; ; i++) {
    yield i;
  }
}

/** Keeps the items that pass the test; it reads its input only when it is read. */
function* filter<T>(items: Iterable<T>, test: (item: T) => boolean): Generator<T> {
  for (const item of items) {
    if (test(item)) {
      yield item;
    }
  }
}

/** Returns the infinite lazy sequence of the prime numbers. */
export function primes(): Generator<number> {
  return filter(naturals(2), isPrime);
}

/** Returns the first n prime numbers. */
export function firstPrimes(n: number): number[] {
  const out: number[] = [];
  if (n === 0) {
    return out;
  }
  // The break stops the pipeline after n items.
  for (const p of primes()) {
    out.push(p);
    if (out.length === n) {
      break;
    }
  }
  return out;
}
