// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

/**
 * Returns n, n / 2, n / 4, ... without the remainders, while the value is greater than 0.
 * @param {number} n
 * @returns {number[]}
 */
export function halvings(n) {
  const out = [];
  while (n > 0) {
    out.push(n);
    // n / 2 can have a fraction, so drop it.
    n = Math.trunc(n / 2);
  }
  return out;
}
