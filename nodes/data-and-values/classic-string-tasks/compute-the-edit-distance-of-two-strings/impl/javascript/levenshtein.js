// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

/**
 * Returns the minimum number of one-character insertions, deletions and
 * substitutions that change a into b. Two rows of the table are kept.
 */
export function levenshtein(a, b) {
  let prev = Array.from({ length: b.length + 1 }, (_, j) => j);
  let cur = new Array(b.length + 1).fill(0);
  for (let i = 1; i <= a.length; i++) {
    cur[0] = i;
    for (let j = 1; j <= b.length; j++) {
      const cost = a[i - 1] === b[j - 1] ? 0 : 1;
      cur[j] = Math.min(prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + cost);
    }
    [prev, cur] = [cur, prev];
  }
  return prev[b.length];
}
