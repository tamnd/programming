// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.
import assert from "node:assert/strict";
import { test } from "node:test";

import { safeDiv } from "./safe_div.ts";

const MAX = 9223372036854775807n;
const MIN = -9223372036854775808n;

const cases: [string, bigint, bigint, bigint | null][] = [
  ["exact", 12n, 4n, 3n],
  ["remainder-dropped", 7n, 2n, 3n],
  ["zero-dividend", 0n, 5n, 0n],
  ["zero-divisor", 5n, 0n, null],
  ["negative-by-zero", -5n, 0n, null],
  ["zero-by-zero", 0n, 0n, null],
  ["negative-dividend", -7n, 2n, -3n],
  ["negative-divisor", 7n, -2n, -3n],
  ["both-negative", -7n, -2n, 3n],
  ["min-by-minus-one", MIN, -1n, null],
  ["min-by-one", MIN, 1n, MIN],
  ["max-by-minus-one", MAX, -1n, -MAX],
  ["min-by-max", MIN, MAX, -1n],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    assert.equal(safeDiv(a, b), want);
  });
}
