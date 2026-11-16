// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { gcdAndLcm } from "./gcd.ts";

const cases: [string, bigint, bigint, [bigint, bigint]][] = [
  ["common-factor", 12n, 18n, [6n, 36n]],
  ["coprime", 17n, 5n, [1n, 85n]],
  ["euclid-example", 48n, 180n, [12n, 720n]],
  ["zero-and-number", 0n, 7n, [7n, 0n]],
  ["both-zero", 0n, 0n, [0n, 0n]],
  ["negative", -12n, 18n, [6n, 36n]],
  ["both-negative", -4n, -6n, [2n, 12n]],
  ["equal", 9n, 9n, [9n, 9n]],
  ["one", 1n, 1000000007n, [1n, 1000000007n]],
  ["multiple", 5n, 25n, [5n, 25n]],
  ["fibonacci", 832040n, 514229n, [1n, 427859097160n]],
  ["big-lcm", 3000000000000000000n, 6000000000000000000n, [3000000000000000000n, 6000000000000000000n]],
  ["largest", 9223372036854775807n, -9223372036854775807n, [9223372036854775807n, 9223372036854775807n]],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    assert.deepEqual(gcdAndLcm(a, b), want);
    assert.deepEqual(gcdAndLcm(b, a), want);
  });
}
