// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

/**
 * @param {number[][]} lists
 * @returns {number[][]}
 */
export function cartesianProduct(lists) {
  // Start with one empty tuple, and add the items of each list in turn.
  return lists.reduce(
    (tuples, xs) => tuples.flatMap((t) => xs.map((x) => [...t, x])),
    [[]],
  );
}
