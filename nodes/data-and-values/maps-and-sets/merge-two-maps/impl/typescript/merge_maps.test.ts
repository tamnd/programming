// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { mergeMaps } from "./merge_maps.ts";

const cases: [string, Map<string, number>, Map<string, number>, [string, number][]][] = [
  ["prices", new Map([["apple", 3], ["pear", 5]]), new Map([["pear", 6], ["fig", 2]]), [["apple", 3], ["fig", 2], ["pear", 6]]],
  ["disjoint", new Map([["a", 1]]), new Map([["b", 2]]), [["a", 1], ["b", 2]]],
  ["empty-base", new Map<string, number>(), new Map([["a", 1]]), [["a", 1]]],
  ["empty-override", new Map([["a", 1]]), new Map<string, number>(), [["a", 1]]],
  ["both-empty", new Map<string, number>(), new Map<string, number>(), []],
  ["same-keys", new Map([["a", 1], ["b", 2]]), new Map([["a", 9], ["b", 8]]), [["a", 9], ["b", 8]]],
  ["zero-wins", new Map([["a", 5]]), new Map([["a", 0]]), [["a", 0]]],
  ["case", new Map([["a", 1]]), new Map([["A", 2]]), [["A", 2], ["a", 1]]],
];

for (const [name, base, override, want] of cases) {
  test(name, () => {
    const before = [[...base], [...override]];
    assert.deepEqual(mergeMaps(base, override), want);
    assert.deepEqual([[...base], [...override]], before);
  });
}
