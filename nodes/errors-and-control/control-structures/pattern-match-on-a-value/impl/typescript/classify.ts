// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

/** Names the place of the point (x, y): origin, x-axis, y-axis, diagonal or other. */
export function classify(x: number, y: number): string {
  // JavaScript has no match statement, so an if chain tests the patterns in
  // order. The first test that is true gives the result.
  if (x === 0 && y === 0) {
    return "origin";
  }
  if (y === 0) {
    return "x-axis";
  }
  if (x === 0) {
    return "y-axis";
  }
  if (x === y) {
    return "diagonal";
  }
  return "other";
}
