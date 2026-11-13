// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

export function zipPairs(a: readonly number[], b: readonly number[]): [number, number][] {
  // Stop at the end of the shorter array. An index past the end gives undefined.
  const n = Math.min(a.length, b.length);
  return Array.from({ length: n }, (_, i) => [a[i], b[i]] as [number, number]);
}
