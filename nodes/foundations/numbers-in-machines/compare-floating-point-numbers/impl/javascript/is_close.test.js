// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.
import assert from "node:assert/strict";
import { test } from "node:test";

import { isClose } from "./is_close.js";

const cases = [
  ["equal", 1.5, 1.5, 1e-9, 0.0, true],
  ["sum-of-tenths", 0.30000000000000004, 0.3, 1e-9, 0.0, true],
  ["different", 1.0, 1.1, 1e-9, 0.0, false],
  ["large-values", 10000000000.0, 10000000001.0, 1e-9, 0.0, true],
  ["zero-and-zero", 0.0, 0.0, 0.0, 0.0, true],
  ["one-ulp-apart", 1.0, 1.0000000000000002, 0.0, 0.0, false],
  ["near-zero-relative", 1e-12, 0.0, 1e-9, 0.0, false],
  ["near-zero-absolute", 1e-12, 0.0, 1e-9, 1e-9, true],
  ["opposite-signs", 1e-10, -1e-10, 1e-9, 0.0, false],
  ["opposite-signs-absolute", 1e-10, -1e-10, 1e-9, 1e-9, true],
  ["on-the-boundary", 10.0, 9.0, 0.1, 0.0, true],
  ["swapped-boundary", 9.0, 10.0, 0.1, 0.0, true],
  ["past-the-boundary", 10.0, 8.875, 0.1, 0.0, false],
  ["negative-values", -1.0, -1.0000000001, 1e-9, 0.0, true],
];

for (const [name, a, b, relTol, absTol, want] of cases) {
  test(name, () => {
    assert.equal(isClose(a, b, relTol, absTol), want);
    assert.equal(isClose(b, a, relTol, absTol), want);
  });
}

test("NaN and infinities", () => {
  assert.equal(isClose(NaN, NaN, 1e-9, 0), false);
  assert.equal(isClose(Infinity, Infinity, 1e-9, 0), true);
  assert.equal(isClose(Infinity, -Infinity, 1e-9, 0), false);
  assert.equal(isClose(Infinity, 1e308, 1e-9, 0), false);
});

test("equality and Number.EPSILON", () => {
  assert.notEqual(0.1 + 0.2, 0.3);
  assert.equal(1 + Number.EPSILON, 1.0000000000000002);
});
