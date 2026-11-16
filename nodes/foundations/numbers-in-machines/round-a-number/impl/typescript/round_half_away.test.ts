// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.
import assert from "node:assert/strict";
import { test } from "node:test";

import { roundHalfAway } from "./round_half_away.ts";

const cases: [string, number, number][] = [
  ["down", 2.4, 2],
  ["up", 2.6, 3],
  ["negative", -2.6, -3],
  ["integer", 7.0, 7],
  ["half-even-up", 2.5, 3],
  ["half-odd-up", 1.5, 2],
  ["half-zero", 0.5, 1],
  ["half-negative", -2.5, -3],
  ["half-negative-small", -0.5, -1],
  ["negative-below-half", -0.4, 0],
  ["near-half", 0.49999999999999994, 0],
  ["zero", 0.0, 0],
  ["large-odd", 4503599627370497.0, 4503599627370497],
  ["large-half-negative", -1000000000000000.5, -1000000000000001],
];

for (const [name, x, want] of cases) {
  test(name, () => {
    // Object.is tells 0 from -0.
    assert.ok(Object.is(roundHalfAway(x), want), `${name}: got ${roundHalfAway(x)}`);
  });
}

test("Math.round rounds a half toward +Infinity", () => {
  assert.equal(Math.round(2.5), 3);
  assert.ok(Object.is(Math.round(-2.5), -2));
  assert.ok(Object.is(Math.round(-0.4), -0));
});

test("toFixed rounds the binary value", () => {
  assert.equal((2.55).toFixed(1), "2.5");
  assert.equal((2.35).toFixed(1), "2.4");
});
