// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { Rectangle, rectangleReport } from "./rectangle.ts";

const cases: [string, [number, number, number], number[]][] = [
  ["three-by-four", [3, 4, 2], [12, 14, 48, 28]],
  ["square", [5, 5, 3], [25, 20, 225, 60]],
  ["zero-side", [0, 7, 2], [0, 14, 0, 28]],
  ["zero-factor", [3, 4, 0], [12, 14, 0, 0]],
  ["identity", [3, 4, 1], [12, 14, 12, 14]],
  ["limit", [1000, 1000, 46], [1000000, 4000, 2116000000, 184000]],
];

for (const [name, args, want] of cases) {
  test(name, () => {
    assert.deepEqual(rectangleReport(...args), want);
  });
}

test("instances have their own fields", () => {
  const a = new Rectangle(1, 2);
  const b = new Rectangle(1, 2);
  a.scale(3);
  assert.deepEqual([a.width, a.height], [3, 6]);
  assert.deepEqual([b.width, b.height], [1, 2]);
  assert.equal(a instanceof Rectangle, true);
});
