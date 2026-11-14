// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { toggle } from "./toggle.ts";

const cases: [string, string[], string[]][] = [
  ["colors", ["red", "blue", "red", "green"], ["blue", "green"]],
  ["fresh", ["b", "a"], ["a", "b"]],
  ["empty", [], []],
  ["two-times", ["a", "a"], []],
  ["three-times", ["a", "a", "a"], ["a"]],
  ["case", ["a", "A"], ["A", "a"]],
  ["prefix", ["ab", "a"], ["a", "ab"]],
];

for (const [name, words, want] of cases) {
  test(name, () => {
    assert.deepEqual(toggle(words), want);
  });
}
