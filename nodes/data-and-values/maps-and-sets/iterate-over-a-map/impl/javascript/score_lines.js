// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

/**
 * @param {Map<string, number>} scores
 * @returns {string[]}
 */
export function scoreLines(scores) {
  const lines = [];
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
