// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.
import assert from "node:assert/strict";
import { test } from "node:test";

import { checkedAdd } from "./checked_add.js";

const MAX = 9223372036854775807n;
const MIN = -9223372036854775808n;

const cases = [
  ["small", 2n, 3n, 5n],
  ["negative", -7n, 4n, -3n],
  ["beyond-32-bit", 4000000000n, 5000000000n, 9000000000n],
  ["max-plus-zero", MAX, 0n, MAX],
  ["max-plus-one", MAX, 1n, null],
  ["max-plus-max", MAX, MAX, null],
  ["min-plus-zero", MIN, 0n, MIN],
  ["min-minus-one", MIN, -1n, null],
  ["min-plus-min", MIN, MIN, null],
  ["min-plus-max", MIN, MAX, -1n],
  ["max-minus-one", MAX, -1n, MAX - 1n],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    assert.equal(checkedAdd(a, b), want);
  });
}

test("a BigInt and a number do not mix", () => {
  assert.throws(() => checkedAdd(1n, 1), TypeError);
});
