// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

export function total(items: readonly number[]): number {
  // The initial value 0 is the result for an empty array. Without it,
  // reduce throws a TypeError for an empty array.
  return items.reduce((acc, x) => acc + x, 0);
}
