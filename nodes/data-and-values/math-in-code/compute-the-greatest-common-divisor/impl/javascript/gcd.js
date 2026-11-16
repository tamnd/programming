// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

const abs = (x) => (x < 0n ? -x : x);

/** Returns [gcd, lcm] of two BigInt values with Euclid's algorithm. */
export function gcdAndLcm(a, b) {
  let x = abs(a);
  let y = abs(b);
  while (y !== 0n) {
    [x, y] = [y, x % y];
  }
  const lcm = a === 0n || b === 0n ? 0n : (abs(a) / x) * abs(b);
  return [x, lcm];
}
