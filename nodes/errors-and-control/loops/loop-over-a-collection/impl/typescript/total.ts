// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

export function total(items: readonly number[]): number {
  let s = 0;
  // for...of gives the items. for...in would give the keys as strings.
  for (const x of items) {
    s += x;
  }
  return s;
}
