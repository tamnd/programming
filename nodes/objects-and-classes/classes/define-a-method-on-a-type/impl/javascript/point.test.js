// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { Point, pointReport } from "./point.js";

const cases = [
  ["right-and-up", [1, 2, 3, 4], [4, 6, 7, 10]],
  ["mixed-signs", [5, -1, -2, 3], [3, 2, 5, 5]],
  ["zero-move", [7, -3, 0, 0], [7, -3, 0, 10]],
  ["negative", [-2, -5, -3, -1], [-5, -6, 4, 11]],
  ["origin", [2, 3, -2, -3], [0, 0, 5, 0]],
  ["limit", [100000000, -100000000, 100000000, -100000000], [200000000, -200000000, 200000000, 400000000]],
];

for (const [name, args, want] of cases) {
  test(name, () => {
    assert.deepEqual(pointReport(...args), want);
  });
}

test("translate returns another point", () => {
  const p = new Point(1, 2);
  const q = p.translate(3, 4);
  assert.deepEqual([p.x, p.y, q.x, q.y], [1, 2, 4, 6]);
  assert.throws(() => {
    p.x = 9;
  }, TypeError);
});
