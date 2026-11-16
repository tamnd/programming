// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.
// A discriminated union: the kind field tells which variant a value is, and
// each variant has its own fields.
export type Shape =
  | { readonly kind: "square"; readonly side: number }
  | { readonly kind: "rectangle"; readonly width: number; readonly height: number }
  | { readonly kind: "triangle"; readonly a: number; readonly b: number; readonly c: number };

export function makeShape(kind: string, sides: readonly number[]): Shape | null {
  if (sides.some((s) => s <= 0)) return null;
  if (kind === "square" && sides.length === 1) {
    return { kind, side: sides[0] };
  }
  if (kind === "rectangle" && sides.length === 2) {
    return { kind, width: sides[0], height: sides[1] };
  }
  if (kind === "triangle" && sides.length === 3) {
    const [a, b, c] = sides;
    if (a < b + c && b < a + c && c < a + b) return { kind, a, b, c };
  }
  return null;
}

export function shapePerimeter(shape: Shape): number {
  switch (shape.kind) {
    case "square":
      return 4 * shape.side;
    case "rectangle":
      return 2 * (shape.width + shape.height);
    case "triangle":
      return shape.a + shape.b + shape.c;
    default: {
      // Every variant is handled, so shape has the type never here.
      const unreachable: never = shape;
      return unreachable;
    }
  }
}

export function perimeter(kind: string, sides: readonly number[]): number | null {
  const shape = makeShape(kind, sides);
  return shape === null ? null : shapePerimeter(shape);
}
