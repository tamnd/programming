// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

export class Rectangle {
  // The fields have types, and the compiler checks that the constructor sets them.
  width: number;
  height: number;

  constructor(width: number, height: number) {
    this.width = width;
    this.height = height;
  }

  area(): number {
    return this.width * this.height;
  }

  perimeter(): number {
    return 2 * (this.width + this.height);
  }

  /** Multiplies both sides by factor. This changes the instance. */
  scale(factor: number): void {
    this.width *= factor;
    this.height *= factor;
  }
}

export function rectangleReport(width: number, height: number, factor: number): number[] {
  const r = new Rectangle(width, height);
  const before = [r.area(), r.perimeter()];
  r.scale(factor);
  return [...before, r.area(), r.perimeter()];
}
