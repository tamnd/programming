// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

export type SetOps = [string[], string[], string[], string[], boolean];

// compareSets returns the union, intersection, difference and symmetric
// difference of the sets of a and b, each sorted, and whether the set of a is a
// subset of the set of b.
export function compareSets(a: readonly string[], b: readonly string[]): SetOps {
  const setA = new Set(a);
  const setB = new Set(b);
  // The default sort compares UTF-16 code units, which is the byte order for ASCII.
  const sorted = (s: Set<string>): string[] => [...s].sort();
  return [
    sorted(setA.union(setB)),
    sorted(setA.intersection(setB)),
    sorted(setA.difference(setB)),
    sorted(setA.symmetricDifference(setB)),
    setA.isSubsetOf(setB),
  ];
}
