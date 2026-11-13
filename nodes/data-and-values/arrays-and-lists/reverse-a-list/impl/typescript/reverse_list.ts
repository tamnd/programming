// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

export function reverseList(items: readonly number[]): number[] {
  // items.reverse() would change items and return the same array.
  return items.toReversed();
}
