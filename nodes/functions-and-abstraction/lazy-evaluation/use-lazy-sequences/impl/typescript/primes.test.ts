// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { firstPrimes, primes } from "./primes.ts";

const cases: [string, number, number[]][] = [
  ["five", 5, [2, 3, 5, 7, 11]],
  ["ten", 10, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]],
  ["twenty-five", 25, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]],
  ["zero", 0, []],
  ["one", 1, [2]],
  ["two", 2, [2, 3]],
  ["past-a-gap", 31, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127]],
];

for (const [name, n, want] of cases) {
  test(name, () => {
    assert.deepEqual(firstPrimes(n), want);
  });
}

test("primes is lazy", () => {
  const seq = primes();
  assert.equal(seq.next().value, 2);
  assert.equal(seq.next().value, 3);
  assert.equal(seq.next().value, 5);
});
