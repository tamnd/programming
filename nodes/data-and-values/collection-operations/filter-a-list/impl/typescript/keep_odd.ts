// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

export function keepOdd(items: readonly number[]): number[] {
  // Use !== 0, not === 1: the remainder of a negative item can be negative.
  return items.filter((x) => x % 2 !== 0);
}
