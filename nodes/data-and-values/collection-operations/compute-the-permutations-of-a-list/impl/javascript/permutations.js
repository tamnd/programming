// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

/**
 * @param {number[]} items
 * @returns {number[][]}
 */
export function permutations(items) {
  const out = [];
  const used = items.map(() => false);
  const path = [];
  // visit adds each unused item to path in turn, from the first position
  // to the last, so the orders come in the lexicographic order of positions.
  const visit = () => {
    if (path.length === items.length) {
      out.push([...path]);
      return;
    }
    for (let i = 0; i < items.length; i++) {
      if (used[i]) continue;
      used[i] = true;
      path.push(items[i]);
      visit();
      path.pop();
      used[i] = false;
    }
  };
  visit();
  return out;
}
