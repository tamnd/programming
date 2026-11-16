// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { complexArithmetic } from "./complexarith.js";

const cases = [
  ["textbook", [3, 4], [1, -2], [4, 2, 11, -2, -1, 2, 5, 0.9272952180016122]],
  ["rosetta", [1, 1], [3.14159, 1.25], [4.14159, 2.25, 1.8915899999999999, 4.39159, 0.38414593243590134, 0.16546321590504276, 1.4142135623730951, 0.7853981633974483]],
  ["real-only", [6, 0], [3, 0], [9, 0, 18, 0, 2, 0, 6, 0]],
  ["imaginary-unit", [0, 1], [0, 1], [0, 2, -1, 0, 1, 0, 1, 1.5707963267948966]],
  ["negative-real", [-2, 0], [1, 0], [-1, 0, -2, 0, -2, 0, 2, 3.141592653589793]],
  ["zero", [0, 0], [2, -3], [2, -3, 0, 0, 0, 0, 0, 0]],
  ["negative-phase", [1, -1], [-1, -1], [0, -2, -2, 0, 0, 1, 1.4142135623730951, -0.7853981633974483]],
  ["imaginary-divisor", [2, 4], [0, 2], [2, 6, -8, 4, 2, -1, 4.47213595499958, 1.1071487177940904]],
  ["large-divisor", [1e+150, 1e+150], [1e+155, 1e+155], [1.00001e+155, 1.00001e+155, 0, 2e+305, 9.999999999999999e-06, 0, 1.414213562373095e+150, 0.7853981633974483]],
];

const close = (got, want) => Math.abs(got - want) <= Math.max(1e-9, 1e-12 * Math.abs(want));

for (const [name, a, b, want] of cases) {
  test(name, () => {
    const got = complexArithmetic(a, b);
    assert.equal(got.length, 8);
    got.forEach((g, i) => assert.ok(close(g, want[i]), `item ${i} = ${g}`));
  });
}
