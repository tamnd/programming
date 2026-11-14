// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { compareSets, type SetOps } from "./compare_sets.ts";

const cases: [string, string[], string[], SetOps][] = [
  ["colors", ["red", "blue", "green"], ["blue", "yellow"], [["blue", "green", "red", "yellow"], ["blue"], ["green", "red"], ["green", "red", "yellow"], false]],
  ["subset", ["b"], ["a", "b", "c"], [["a", "b", "c"], ["b"], [], ["a", "c"], true]],
  ["both-empty", [], [], [[], [], [], [], true]],
  ["empty-a", [], ["x"], [["x"], [], [], ["x"], true]],
  ["empty-b", ["x"], [], [["x"], [], ["x"], ["x"], false]],
  ["equal", ["a", "b"], ["b", "a"], [["a", "b"], ["a", "b"], [], [], true]],
  ["disjoint", ["a"], ["b"], [["a", "b"], [], ["a"], ["a", "b"], false]],
  ["duplicates", ["a", "a", "b"], ["b", "b"], [["a", "b"], ["b"], ["a"], ["a"], false]],
  ["case", ["a"], ["A"], [["A", "a"], [], ["a"], ["A", "a"], false]],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    assert.deepEqual(compareSets(a, b), want);
  });
}
