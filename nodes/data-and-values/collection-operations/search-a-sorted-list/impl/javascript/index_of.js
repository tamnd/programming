// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

/**
 * @param {number[]} items
 * @param {number} target
 * @returns {number}
 */
export function indexOf(items, target) {
  // Find the first index lo with items[lo] >= target. The search keeps
  // items[i] < target for each i < lo, and items[i] >= target for each i >= hi.
  let lo = 0;
  let hi = items.length;
  while (lo < hi) {
    const mid = lo + Math.floor((hi - lo) / 2);
    if (items[mid] < target) {
      lo = mid + 1;
    } else {
      hi = mid;
    }
  }
  return lo < items.length && items[lo] === target ? lo : -1;
}
