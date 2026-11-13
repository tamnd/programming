// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

/**
 * @param {string[]} items
 * @returns {number}
 */
export function length(items) {
  // length is one more than the highest index. With no holes, that is the
  // number of items.
  return items.length;
}
