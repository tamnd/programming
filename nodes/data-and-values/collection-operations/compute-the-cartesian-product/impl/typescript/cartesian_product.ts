// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

export function cartesianProduct(lists: readonly (readonly number[])[]): number[][] {
  // Start with one empty tuple, and add the items of each list in turn.
  return lists.reduce<number[][]>(
    (tuples, xs) => tuples.flatMap((t) => xs.map((x) => [...t, x])),
    [[]],
  );
}
