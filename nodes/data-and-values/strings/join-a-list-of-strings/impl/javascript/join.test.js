// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { join, joinNumbers } from "./join.js";

const cases = [
  ["comma", ["a", "b", "c"], ",", "a,b,c"],
  ["comma-space", ["red", "green", "blue"], ", ", "red, green, blue"],
  ["words", ["Hello", "world"], " ", "Hello world"],
  ["empty-list", [], ",", ""],
  ["single", ["only"], ",", "only"],
  ["empty-items", ["", "", ""], ",", ",,"],
  ["empty-middle", ["a", "", "b"], "-", "a--b"],
  ["empty-sep", ["a", "b", "c"], "", "abc"],
  ["long-sep", ["x", "y"], " -> ", "x -> y"],
  ["non-ascii", ["naïve", "日本"], "·", "naïve·日本"],
  ["astral-sep", ["a", "b"], "\u{1F642}", "a\u{1F642}b"],
  ["sep-in-item", ["a,b", "c"], ",", "a,b,c"],
];

const numberCases = [
  ["numbers", [1, 2, 3], ",", "1,2,3"],
  ["negative", [-1, 0, 1], " ", "-1 0 1"],
  ["empty", [], ",", ""],
  ["single", [42], ",", "42"],
  ["int32-ends", [2147483647, -2147483648], ";", "2147483647;-2147483648"],
];

for (const [name, items, sep, want] of cases) {
  test(name, () => {
    assert.equal(join(items, sep), want);
  });
}

for (const [name, items, sep, want] of numberCases) {
  test("numbers " + name, () => {
    assert.equal(joinNumbers(items, sep), want);
  });
}
