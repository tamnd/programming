// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { levenshtein } from "./levenshtein.js";

const cases = [
  ["kitten-sitting", "kitten", "sitting", 3],
  ["rosettacode", "rosettacode", "raisethysword", 8],
  ["flaw-lawn", "flaw", "lawn", 2],
  ["both-empty", "", "", 0],
  ["empty-left", "", "abc", 3],
  ["empty-right", "abc", "", 3],
  ["equal", "same", "same", 0],
  ["one-substitution", "a", "b", 1],
  ["transposition", "ab", "ba", 2],
  ["case", "Apple", "apple", 1],
  ["disjoint", "abc", "xyz", 3],
  ["prefix", "abc", "abcdef", 3],
  ["intention", "intention", "execution", 5],
  ["space", "a b", "ab", 1],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    assert.equal(levenshtein(a, b), want);
    assert.equal(levenshtein(b, a), want);
  });
}
