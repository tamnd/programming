// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

/** Returns [mean, median] of a non-empty array of numbers. */
export function meanAndMedian(items) {
  const n = items.length;
  const mean = items.reduce((a, x) => a + x, 0) / n;
  // The default sort compares strings, so give a compare function.
  const s = items.toSorted((a, b) => a - b);
  const mid = Math.floor(n / 2);
  const median = n % 2 === 1 ? s[mid] : (s[mid - 1] + s[mid]) / 2;
  return [mean, median];
}
