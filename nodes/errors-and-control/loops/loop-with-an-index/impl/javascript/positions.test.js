// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { positions } from "./positions.js";

const cases = [
  ["two-matches", ["a", "b", "a", "c"], "a", [0, 2]],
  ["words", ["red", "green", "blue", "green"], "green", [1, 3]],
  ["empty", [], "a", []],
  ["none", ["a", "b"], "z", []],
  ["all", ["x", "x", "x"], "x", [0, 1, 2]],
  ["last", ["a", "b", "c"], "c", [2]],
  ["case", ["A", "a"], "a", [1]],
  ["empty-string", ["", "a", ""], "", [0, 2]],
];

for (const [name, items, target, want] of cases) {
  test(name, () => {
    assert.deepEqual(positions(items, target), want);
  });
}
