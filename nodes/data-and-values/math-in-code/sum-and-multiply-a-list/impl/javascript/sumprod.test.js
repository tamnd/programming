// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { sumAndProduct } from "./sumprod.js";

const cases = [
  ["small", [1n, 2n, 3n, 4n], [10n, 24n]],
  ["mixed", [3n, -2n, 5n], [6n, -30n]],
  ["empty", [], [0n, 1n]],
  ["single", [7n], [7n, 7n]],
  ["with-zero", [5n, 0n, 9n], [14n, 0n]],
  ["two-negatives", [-2n, -3n], [-5n, 6n]],
  ["three-negatives", [-1n, -2n, -3n], [-6n, -6n]],
  ["ones", [1n, 1n, 1n], [3n, 1n]],
  ["large-product", [3037000499n, 3037000499n], [6074000998n, 9223372030926249001n]],
  ["to-the-limit", [9223372036854775806n, 1n], [9223372036854775807n, 9223372036854775806n]],
  ["beyond-double", [9007199254740993n], [9007199254740993n, 9007199254740993n]],
  ["min", [-9223372036854775808n], [-9223372036854775808n, -9223372036854775808n]],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    assert.deepEqual(sumAndProduct(items), want);
  });
}
