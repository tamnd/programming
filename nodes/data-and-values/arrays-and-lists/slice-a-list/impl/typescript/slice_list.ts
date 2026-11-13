// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

export function sliceList(items: readonly number[], start: number, stop: number): number[] {
  if (!(Number.isInteger(start) && Number.isInteger(stop) && 0 <= start && start <= stop && stop <= items.length)) {
    throw new RangeError(`range ${start}..${stop} is out of bounds`);
  }
  // slice copies and does not change items. splice would remove the items.
  return items.slice(start, stop);
}
