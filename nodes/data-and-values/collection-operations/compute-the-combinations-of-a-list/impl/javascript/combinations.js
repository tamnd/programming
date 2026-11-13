// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

/**
 * @param {number[]} items
 * @param {number} k
 * @returns {number[][]}
 */
export function combinations(items, k) {
  const out = [];
  const path = [];
  // visit adds each item from position start on to path in turn. It stops
  // when too few items are left to fill path.
  const visit = (start) => {
    if (path.length === k) {
      out.push([...path]);
      return;
    }
    for (let i = start; i <= items.length - (k - path.length); i++) {
      path.push(items[i]);
      visit(i + 1);
      path.pop();
    }
  };
  visit(0);
  return out;
}
