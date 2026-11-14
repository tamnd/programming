// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { lookup } from "./lookup.js";

const cases = [
  ["prices", new Map([["apple", 3], ["pear", 5]]), ["apple", "fig", "pear"], 0, [3, 0, 5]],
  ["negative-fallback", new Map([["a", 1]]), ["b", "a"], -1, [-1, 1]],
  ["empty-table", new Map(), ["a", "b"], 7, [7, 7]],
  ["empty-keys", new Map([["a", 1]]), [], 0, []],
  ["zero-value", new Map([["a", 0]]), ["a", "b"], 5, [0, 5]],
  ["repeat", new Map([["a", 1]]), ["b", "b", "a"], 2, [2, 2, 1]],
  ["case", new Map([["a", 1]]), ["A"], 0, [0]],
];

for (const [name, table, keys, fallback, want] of cases) {
  test(name, () => {
    const before = [...table];
    assert.deepEqual(lookup(table, keys, fallback), want);
    assert.deepEqual([...table], before);
  });
}
