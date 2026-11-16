// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { factorial } from "./factorial.js";

const cases = [
  ["five", 5n, 120n],
  ["ten", 10n, 3628800n],
  ["zero", 0n, 1n],
  ["one", 1n, 1n],
  ["largest-32-bit", 12n, 479001600n],
  ["beyond-32-bit", 13n, 6227020800n],
  ["largest-64-bit", 20n, 2432902008176640000n],
];

for (const [name, n, want] of cases) {
  test(name, () => {
    assert.equal(factorial(n), want);
  });
}

test("negative", () => {
  assert.throws(() => factorial(-1n), RangeError);
});
