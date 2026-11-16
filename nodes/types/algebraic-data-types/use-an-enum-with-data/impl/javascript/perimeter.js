// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.
// JavaScript has no enum with data. Each shape is a frozen object with a
// kind field that tells which other fields it has.
export function makeShape(kind, sides) {
  if (sides.some((s) => s <= 0)) return null;
  if (kind === "square" && sides.length === 1) {
    return Object.freeze({ kind, side: sides[0] });
  }
  if (kind === "rectangle" && sides.length === 2) {
    return Object.freeze({ kind, width: sides[0], height: sides[1] });
  }
  if (kind === "triangle" && sides.length === 3) {
    const [a, b, c] = sides;
    if (a < b + c && b < a + c && c < a + b) {
      return Object.freeze({ kind, a, b, c });
    }
  }
  return null;
}

export function shapePerimeter(shape) {
  switch (shape.kind) {
    case "square":
      return 4 * shape.side;
    case "rectangle":
      return 2 * (shape.width + shape.height);
    case "triangle":
      return shape.a + shape.b + shape.c;
    default:
      throw new TypeError(`not a shape: ${shape.kind}`);
  }
}

export function perimeter(kind, sides) {
  const shape = makeShape(kind, sides);
  return shape === null ? null : shapePerimeter(shape);
}
