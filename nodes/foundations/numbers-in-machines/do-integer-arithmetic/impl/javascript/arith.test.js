// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.
import assert from "node:assert/strict";
import { test } from "node:test";

import { arith } from "./arith.js";

const cases = [
  ["small", 7, 2, 3, [9, 5, 14, 3, 1, 343]],
  ["exact-division", 20, 5, 2, [25, 15, 100, 4, 0, 400]],
  ["negative-dividend", -7, 2, 3, [-5, -9, -14, -3, -1, -343]],
  ["negative-divisor", 7, -2, 2, [5, 9, -14, -3, 1, 49]],
  ["both-negative", -7, -2, 1, [-9, -5, 14, 3, -1, -7]],
  ["zero-dividend", 0, 5, 0, [5, -5, 0, 0, 0, 1]],
  ["zero-power", 5, 3, 0, [8, 2, 15, 1, 2, 1]],
  ["divisor-larger", 3, 7, 1, [10, -4, 21, 0, 3, 3]],
  ["largest", 100000, 100000, 3, [200000, 0, 10000000000, 1, 0, 1e15]],
  ["smallest", -100000, 1, 3, [-99999, -100001, -100000, -100000, 0, -1e15]],
];

for (const [name, a, b, n, want] of cases) {
  test(name, () => {
    // deepEqual with Object.is would tell -0 from 0; compare with ===.
    const got = arith(a, b, n);
    assert.equal(got.length, want.length);
    got.forEach((x, i) => assert.ok(x === want[i], `${name}[${i}]: ${x} != ${want[i]}`));
  });
}

test("caret is not a power", () => {
  assert.equal(2 ^ 3, 1);
  assert.equal(2 ** 3, 8);
});
