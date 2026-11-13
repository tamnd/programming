// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

export function partitionOdd(items: readonly number[]): [number[], number[]] {
  const odd: number[] = [];
  const even: number[] = [];
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
