// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { fibonacci } from "./fibonacci.ts";

const cases: [string, bigint, bigint][] = [
  ["ten", 10n, 55n],
  ["twenty", 20n, 6765n],
  ["thirty", 30n, 832040n],
  ["zero", 0n, 0n],
  ["one", 1n, 1n],
  ["two", 2n, 1n],
  ["largest-32-bit", 46n, 1836311903n],
  ["beyond-32-bit", 47n, 2971215073n],
  ["binet-fails", 71n, 308061521170129n],
  ["largest-64-bit", 92n, 7540113804746346429n],
];

for (const [name, n, want] of cases) {
  test(name, () => {
    assert.equal(fibonacci(n), want);
  });
}

test("big", () => {
  assert.equal(fibonacci(100n), 354224848179261915075n);
});

test("negative", () => {
  assert.throws(() => fibonacci(-1n), RangeError);
});
