// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.
import assert from "node:assert/strict";
import { test } from "node:test";

import { makeShape, perimeter } from "./perimeter.ts";

const cases: [string, string, number[], number | null][] = [
  ["square", "square", [3], 12],
  ["rectangle", "rectangle", [2, 5], 14],
  ["triangle", "triangle", [3, 4, 5], 12],
  ["unknown-kind", "circle", [2], null],
  ["wrong-case", "Square", [3], null],
  ["too-few-sides", "rectangle", [4], null],
  ["too-many-sides", "square", [3, 3], null],
  ["no-sides", "triangle", [], null],
  ["zero-side", "square", [0], null],
  ["negative-side", "rectangle", [-2, 5], null],
  ["not-a-triangle", "triangle", [1, 2, 3], null],
  ["largest", "triangle", [1000000, 1000000, 1000000], 3000000],
];

for (const [name, kind, sides, want] of cases) {
  test(name, () => {
    assert.equal(perimeter(kind, sides), want);
  });
}

test("variants carry their data", () => {
  assert.deepEqual(makeShape("triangle", [3, 4, 5]), { kind: "triangle", a: 3, b: 4, c: 5 });
});
