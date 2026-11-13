// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { groupByLength } from "./group_by_length.js";

const cases = [
  ["fruits", ["apple", "fig", "kiwi", "pear", "plum", "date"], [[3, ["fig"]], [4, ["kiwi", "pear", "plum", "date"]], [5, ["apple"]]]],
  ["letters", ["a", "bb", "c"], [[1, ["a", "c"]], [2, ["bb"]]]],
  ["empty", [], []],
  ["single", ["x"], [[1, ["x"]]]],
  ["same-length", ["cat", "dog", "cow"], [[3, ["cat", "dog", "cow"]]]],
  ["order", ["ccc", "a", "bb"], [[1, ["a"]], [2, ["bb"]], [3, ["ccc"]]]],
  ["duplicates", ["ab", "ab", "c"], [[1, ["c"]], [2, ["ab", "ab"]]]],
  ["long", ["abcdefghij", "abcdefghi", "a"], [[1, ["a"]], [9, ["abcdefghi"]], [10, ["abcdefghij"]]]],
];

for (const [name, words, want] of cases) {
  test(name, () => {
    assert.deepEqual(groupByLength(words), want);
  });
}
