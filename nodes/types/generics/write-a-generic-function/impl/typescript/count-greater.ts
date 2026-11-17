// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

/** Returns how many items are greater than limit. */
export function countGreater<T extends number | string | bigint>(items: readonly T[], limit: T): number {
  let count = 0;
  for (const item of items) {
    if (item > limit) {
      count++;
    }
  }
  return count;
}
