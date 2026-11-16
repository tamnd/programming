// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

/** Returns true if the bigint n is prime, with trial division. */
export function isPrime(n: bigint): boolean {
  if (n < 2n) {
    return false;
  }
  if (n % 2n === 0n) {
    return n === 2n;
  }
  for (let i = 3n; i * i <= n; i += 2n) {
    if (n % i === 0n) {
      return false;
    }
  }
  return true;
}
