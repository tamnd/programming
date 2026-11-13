// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

export function flatten(lists: readonly (readonly number[])[]): number[] {
  // flat() with no argument flattens one level.
  return lists.flat();
}
