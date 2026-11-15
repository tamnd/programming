// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

/** A point on a grid. The fields are readonly, so methods return other points. */
export class Point {
  readonly x: number;
  readonly y: number;

  constructor(x: number, y: number) {
    this.x = x;
    this.y = y;
  }

  /** Returns the point moved by (dx, dy). */
  translate(dx: number, dy: number): Point {
    return new Point(this.x + dx, this.y + dy);
  }

  /** Returns the grid distance |dx| + |dy| to another point. */
  manhattan(other: Point): number {
    return Math.abs(this.x - other.x) + Math.abs(this.y - other.y);
  }
}

const ORIGIN = new Point(0, 0);

export function pointReport(x: number, y: number, dx: number, dy: number): number[] {
  const p = new Point(x, y);
  const q = p.translate(dx, dy);
  return [q.x, q.y, p.manhattan(q), q.manhattan(ORIGIN)];
}
