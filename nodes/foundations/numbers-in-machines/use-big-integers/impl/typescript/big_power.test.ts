// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.
import assert from "node:assert/strict";
import { test } from "node:test";

import { bigPower } from "./big_power.ts";

const cases: [string, number, number, string][] = [
  ["small", 2, 10, "1024"],
  ["two-to-the-100", 2, 100, "1267650600228229401496703205376"],
  ["ten-to-the-30", 10, 30, "1" + "0".repeat(30)],
  ["zero-exponent", 7, 0, "1"],
  ["zero-to-the-zero", 0, 0, "1"],
  ["zero-base", 0, 5, "0"],
  ["minus-one-odd", -1, 199, "-1"],
  ["negative-odd", -3, 41, "-36472996377170786403"],
  ["negative-even", -3, 40, "12157665459056928801"],
  ["int64-max-plus-one", 2, 63, "9223372036854775808"],
  ["uint64-max-plus-one", 2, 64, "18446744073709551616"],
  ["two-to-the-200", 2, 200, "1606938044258990275541962092341162602522202993782792835301376"],
  ["largest", 1000, 200, "1" + "0".repeat(600)],
  ["smallest", -1000, 199, "-1" + "0".repeat(597)],
];

for (const [name, base, exp, want] of cases) {
  test(name, () => {
    assert.equal(bigPower(base, exp), want);
  });
}

test("fixed width", () => {
  const big = 2n ** 63n;
  assert.equal(BigInt("9223372036854775808"), big);
  assert.equal(BigInt.asIntN(64, big), -big);
  assert.equal(BigInt.asUintN(64, big), big);
  assert.equal(Number(2n ** 64n + 1n), 2 ** 64);
  assert.throws(() => BigInt(1.5), RangeError);
});
