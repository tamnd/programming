// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

export function squareAll(items: readonly number[]): number[] {
  // map makes a new array and does not change items.
  return items.map((x) => x * x);
}
