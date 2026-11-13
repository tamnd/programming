// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

export function countItems(items: readonly number[]): [number, number][] {
  // A Map keeps the numbers as numbers. The keys of an object are strings.
  const counts = new Map<number, number>();
  for (const x of items) {
    counts.set(x, (counts.get(x) ?? 0) + 1);
  }
  // Compare the numbers. sort() without a function compares strings.
  return [...counts].sort((p, q) => p[0] - q[0]);
}
