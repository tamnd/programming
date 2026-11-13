// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

/**
 * @param {number[]} items
 * @returns {[number[], number[]]}
 */
export function partitionOdd(items) {
  const odd = [];
  const even = [];
  for (const x of items) {
    // Use !== 0, not === 1: the remainder of a negative item can be negative.
    if (x % 2 !== 0) {
      odd.push(x);
    } else {
      even.push(x);
    }
  }
  return [odd, even];
}
