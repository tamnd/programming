// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

export class Rectangle {
  // Fields declared in the class body. Each instance gets its own copy.
  width;
  height;

  constructor(width, height) {
    this.width = width;
    this.height = height;
  }

  area() {
    return this.width * this.height;
  }

  perimeter() {
    return 2 * (this.width + this.height);
  }

  /** Multiplies both sides by factor. This changes the instance. */
  scale(factor) {
    this.width *= factor;
    this.height *= factor;
  }
}

/**
 * @param {number} width
 * @param {number} height
 * @param {number} factor
 * @returns {number[]}
 */
export function rectangleReport(width, height, factor) {
  const r = new Rectangle(width, height);
  const before = [r.area(), r.perimeter()];
  r.scale(factor);
  return [...before, r.area(), r.perimeter()];
}
