// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

/** Returns the items with sep between each two. */
export function join(items: readonly string[], sep: string): string {
  // join with no argument uses a comma, so the code always passes sep.
  return items.join(sep);
}

/** Returns the decimal form of each item, with sep between each two. */
export function joinNumbers(items: readonly number[], sep: string): string {
  return items.map(String).join(sep);
}
