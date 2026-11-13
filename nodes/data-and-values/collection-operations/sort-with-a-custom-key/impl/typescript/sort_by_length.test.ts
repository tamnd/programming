// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { sortByLength } from "./sort_by_length.ts";

const cases: [string, string[], string[]][] = [
  ["fruits", ["pear", "fig", "apple", "kiwi"], ["fig", "kiwi", "pear", "apple"]],
  ["three", ["bb", "a", "ccc"], ["a", "bb", "ccc"]],
  ["empty", [], []],
  ["single", ["x"], ["x"]],
  ["same-length", ["dog", "cat", "ant"], ["ant", "cat", "dog"]],
  ["duplicates", ["b", "a", "b"], ["a", "b", "b"]],
  ["sorted", ["a", "bb", "ccc"], ["a", "bb", "ccc"]],
  ["reverse", ["ccc", "bb", "a"], ["a", "bb", "ccc"]],
  ["long", ["abcdefghij", "abcdefghi", "z"], ["z", "abcdefghi", "abcdefghij"]],
  ["prefix", ["ab", "a", "abc", "b"], ["a", "b", "ab", "abc"]],
];

for (const [name, words, want] of cases) {
  test(name, () => {
    const given = [...words];
    assert.deepEqual(sortByLength(words), want);
    assert.deepEqual(words, given);
  });
}
