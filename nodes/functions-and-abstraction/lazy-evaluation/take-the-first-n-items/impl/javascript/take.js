// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

/**
 * Returns a copy of the first n items; a shorter array gives all its items.
 * @param {number[]} items
 * @param {number} n at least 0
 * @returns {number[]}
 */
export function take(items, n) {
  // slice stops at the end of the array, so n can be larger than the length.
  return items.slice(0, n);
}

/** The infinite sequence start, start + 1, ... It runs only when it is read. */
function* naturals(start) {
  for (let i = start; ; i++) {
    yield i;
  }
}

/**
 * Returns the first n items of the infinite sequence start, start + 1, ...
 * @param {number} start
 * @param {number} n at least 0
 * @returns {number[]}
 */
export function takeNaturals(start, n) {
  // Iterator.prototype.take reads only n items from the generator.
  return naturals(start).take(n).toArray();
}
