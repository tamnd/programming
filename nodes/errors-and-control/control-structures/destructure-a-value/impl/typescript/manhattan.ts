// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

/** A point as a pair [x, y]. */
export type Point = [number, number];

/** Returns the Manhattan distance between the points p and q. */
export function manhattan(p: Point, q: Point): number {
  const [x1, y1] = p;
  const [x2, y2] = q;
  return Math.abs(x1 - x2) + Math.abs(y1 - y2);
}
