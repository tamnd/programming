// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

/** Returns how many items are greater than limit. The items and limit must be of one type that > orders. */
export function countGreater(items, limit) {
  let count = 0;
  for (const item of items) {
    if (item > limit) {
      count++;
    }
  }
  return count;
}
