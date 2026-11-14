// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

/**
 * Returns the union, intersection, difference and symmetric difference of the
 * sets of a and b, each sorted, and whether the set of a is a subset of the set of b.
 * @param {string[]} a
 * @param {string[]} b
 * @returns {[string[], string[], string[], string[], boolean]}
 */
export function compareSets(a, b) {
  const setA = new Set(a);
  const setB = new Set(b);
  // The default sort compares UTF-16 code units, which is the byte order for ASCII.
  const sorted = (s) => [...s].sort();
  return [
    sorted(setA.union(setB)),
    sorted(setA.intersection(setB)),
    sorted(setA.difference(setB)),
    sorted(setA.symmetricDifference(setB)),
    setA.isSubsetOf(setB),
  ];
}
