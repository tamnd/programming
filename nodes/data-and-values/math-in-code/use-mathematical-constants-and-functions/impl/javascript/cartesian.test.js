// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { toCartesian } from "./cartesian.js";

const cases = [
  ["east", 1, 0, [1, 0]],
  ["north", 2, 90, [0, 2]],
  ["west", 2, 180, [-2, 0]],
  ["diagonal", 1, 45, [0.7071067811865476, 0.7071067811865476]],
  ["thirty-degrees", 2, 30, [1.7320508075688772, 1]],
  ["zero-radius", 0, 123, [0, 0]],
  ["south", 1, 270, [0, -1]],
  ["negative-angle", 1, -90, [0, -1]],
  ["full-turn", 1, 360, [1, 0]],
  ["two-turns", 3, 720, [3, 0]],
  ["negative-radius", -1, 0, [-1, 0]],
  ["large-radius", 1000000, 60, [500000, 866025.4037844386]],
];

const close = (got, want) => Math.abs(got - want) <= Math.max(1e-9, 1e-12 * Math.abs(want));

for (const [name, r, degrees, want] of cases) {
  test(name, () => {
    const [x, y] = toCartesian(r, degrees);
    assert.ok(close(x, want[0]), `x = ${x}`);
    assert.ok(close(y, want[1]), `y = ${y}`);
  });
}

test("constants", () => {
  assert.ok(close(Math.atan2(0, -1), Math.PI));
  assert.equal(Math.sqrt(16), 4);
  assert.equal(2 ** 10, 1024);
  assert.equal(2 ^ 10, 8);
});
