// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

/** A point on a grid. The constructor freezes it, so methods return other points. */
export class Point {
  constructor(x, y) {
    this.x = x;
    this.y = y;
    Object.freeze(this);
  }

  /** Returns the point moved by (dx, dy). */
  translate(dx, dy) {
    return new Point(this.x + dx, this.y + dy);
  }

  /** Returns the grid distance |dx| + |dy| to another point. */
  manhattan(other) {
    return Math.abs(this.x - other.x) + Math.abs(this.y - other.y);
  }
}

const ORIGIN = new Point(0, 0);

export function pointReport(x, y, dx, dy) {
  const p = new Point(x, y);
  const q = p.translate(dx, dy);
  return [q.x, q.y, p.manhattan(q), q.manhattan(ORIGIN)];
}
