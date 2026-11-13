// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

export function contains(items: readonly number[], value: number): boolean {
  // includes returns a boolean. indexOf returns a position, and 0 is falsy.
  return items.includes(value);
}
