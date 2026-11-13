// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

/**
 * @param {number[]} items
 * @returns {number}
 */
export function largest(items) {
  if (items.length === 0) {
    throw new RangeError("largest: the array is empty");
  }
  // A loop, not Math.max(...items): the spread passes each item as an
  // argument, and a very long array can go past the limit.
  let best = items[0];
  for (const x of items) {
    if (x > best) {
      best = x;
    }
  }
  return best;
}
