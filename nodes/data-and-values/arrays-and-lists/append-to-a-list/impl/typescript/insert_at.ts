// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

export function insertAt(items: readonly number[], index: number, value: number): number[] {
  if (!Number.isInteger(index) || index < 0 || index > items.length) {
    throw new RangeError(`index ${index} is out of range`);
  }
  // slice copies, so items does not change. splice would change it in place.
  return [...items.slice(0, index), value, ...items.slice(index)];
}
