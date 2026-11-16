// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

// JavaScript has no user operators, so the type has methods in place of + and ==.
export class Vec2 {
  constructor(x, y) {
    this.x = x;
    this.y = y;
    Object.freeze(this);
  }

  add(other) {
    return new Vec2(this.x + other.x, this.y + other.y);
  }

  equals(other) {
    return other instanceof Vec2 && this.x === other.x && this.y === other.y;
  }
}

export function vectorSum(xs, ys) {
  if (xs.length !== ys.length) {
    throw new RangeError("the lists must have the same length");
  }
  const total = xs.reduce((acc, x, i) => acc.add(new Vec2(x, ys[i])), new Vec2(0, 0));
  return [total.x, total.y];
}
