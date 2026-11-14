// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

export function scoreLines(scores: ReadonlyMap<string, number>): string[] {
  const lines: string[] = [];
  // A Map gives its keys in insertion order. sort() without a function
  // compares strings by their UTF-16 code units, which is byte order for ASCII.
  for (const key of [...scores.keys()].sort()) {
    lines.push(`${key}=${scores.get(key)}`);
  }
  let total = 0;
  for (const value of scores.values()) {
    total += value;
  }
  lines.push(`total=${total}`);
  return lines;
}
