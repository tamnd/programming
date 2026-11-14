// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { countPoints } from "./count_points.ts";

const cases: [string, [number, number][], [number, number, number][]][] = [
  ["clicks", [[1, 2], [3, 4], [1, 2]], [[1, 2, 2], [3, 4, 1]]],
  ["swap", [[2, 1], [1, 2]], [[1, 2, 1], [2, 1, 1]]],
  ["empty", [], []],
  ["one", [[0, 0]], [[0, 0, 1]]],
  ["negative", [[-1, 0], [0, -1], [-1, 0]], [[-1, 0, 2], [0, -1, 1]]],
  ["joined-digits", [[1, 23], [12, 3]], [[1, 23, 1], [12, 3, 1]]],
  ["all-same", [[5, 5], [5, 5], [5, 5]], [[5, 5, 3]]],
];

for (const [name, points, want] of cases) {
  test(name, () => {
    assert.deepEqual(countPoints(points), want);
  });
}
