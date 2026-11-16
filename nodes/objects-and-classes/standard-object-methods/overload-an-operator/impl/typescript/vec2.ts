// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

// JavaScript has no user operators, so the type has methods in place of + and ==.
export class Vec2 {
  readonly x: number;
  readonly y: number;

  constructor(x: number, y: number) {
    this.x = x;
    this.y = y;
    Object.freeze(this);
  }

  add(other: Vec2): Vec2 {
    return new Vec2(this.x + other.x, this.y + other.y);
  }

  equals(other: unknown): boolean {
    return other instanceof Vec2 && this.x === other.x && this.y === other.y;
  }
}

export function vectorSum(xs: number[], ys: number[]): number[] {
  if (xs.length !== ys.length) {
    throw new RangeError("the lists must have the same length");
  }
  const total = xs.reduce((acc, x, i) => acc.add(new Vec2(x, ys[i])), new Vec2(0, 0));
  return [total.x, total.y];
}
