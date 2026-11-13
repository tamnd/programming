// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

/**
 * Returns a new array of items[start] up to, but not including, items[stop].
 * @param {number[]} items
 * @param {number} start
 * @param {number} stop
 * @returns {number[]}
 */
export function sliceList(items, start, stop) {
  if (!(Number.isInteger(start) && Number.isInteger(stop) && 0 <= start && start <= stop && stop <= items.length)) {
    throw new RangeError(`range ${start}..${stop} is out of bounds`);
  }
  // slice copies and does not change items. splice would remove the items.
  return items.slice(start, stop);
}
