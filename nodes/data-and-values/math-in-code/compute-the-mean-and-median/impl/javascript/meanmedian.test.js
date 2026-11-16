// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { meanAndMedian } from "./meanmedian.js";

const cases = [
  ["odd", [3, 1, 2], [2, 2]],
  ["even", [4, 1, 3, 2], [2.5, 2.5]],
  ["skewed", [1, 2, 3, 4, 100], [22, 3]],
  ["single", [5], [5, 5]],
  ["two-items", [1, 2], [1.5, 1.5]],
  ["negative", [-3, -1, -2], [-2, -2]],
  ["unsorted-even", [10, 2, 8, 4], [6, 6]],
  ["duplicates", [1, 1, 1, 5], [2, 1]],
  ["not-an-integer", [1, 2, 2], [1.6666666666666667, 2]],
  ["large", [9223372036854775807, 9223372036854775807], [9223372036854775808, 9223372036854775808]],
];

const close = (got, want) => Math.abs(got - want) <= Math.max(1e-9, 1e-12 * Math.abs(want));

for (const [name, items, want] of cases) {
  test(name, () => {
    const before = [...items];
    const [mean, median] = meanAndMedian(items);
    assert.ok(close(mean, want[0]), `mean = ${mean}`);
    assert.ok(close(median, want[1]), `median = ${median}`);
    assert.deepEqual(items, before);
  });
}
