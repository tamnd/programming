// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

export function countPoints(points: readonly (readonly [number, number])[]): [number, number, number][] {
  // A Map compares object keys by identity, so two arrays for one point would
  // be two keys. A string with a separator gives one key for each point.
  const counts = new Map<string, [number, number, number]>();
  for (const [x, y] of points) {
    const key = `${x},${y}`;
    const entry = counts.get(key);
    if (entry === undefined) {
      counts.set(key, [x, y, 1]);
    } else {
      entry[2]++;
    }
  }
  return [...counts.values()].sort((a, b) => a[0] - b[0] || a[1] - b[1]);
}
