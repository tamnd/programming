// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { isPrime } from "./prime.ts";

const cases: [string, bigint, boolean][] = [
  ["small-prime", 17n, true],
  ["small-composite", 15n, false],
  ["even", 100n, false],
  ["odd-composite", 91n, false],
  ["negative", -7n, false],
  ["zero", 0n, false],
  ["one", 1n, false],
  ["two", 2n, true],
  ["three", 3n, true],
  ["square-of-prime", 25n, false],
  ["large-square", 4293001441n, false],
  ["carmichael", 561n, false],
  ["large-prime", 1000000007n, true],
  ["mersenne-prime", 2147483647n, true],
  ["large-composite", 2147483649n, false],
];

for (const [name, n, want] of cases) {
  test(name, () => {
    assert.equal(isPrime(n), want);
  });
}
