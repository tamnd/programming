// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

export function buildMap(pairs: readonly [string, number][], remove: readonly string[]): [string, number][] {
  const map = new Map<string, number>();
  for (const [key, value] of pairs) {
    // set adds the key, or replaces the value of a key that is there.
    map.set(key, value);
  }
  for (const key of remove) {
    // delete returns false for a key that is not there.
    map.delete(key);
  }
  // Compare the keys by their code units. localeCompare would put "a" before "B".
  return [...map].sort((p, q) => (p[0] < q[0] ? -1 : p[0] > q[0] ? 1 : 0));
}
