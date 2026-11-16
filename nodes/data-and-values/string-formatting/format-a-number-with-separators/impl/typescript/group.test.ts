// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { groupThousands } from "./group.ts";

const cases: [string, number | bigint, string][] = [
  ["million", 1234567, "1,234,567"],
  ["thousand", 1000, "1,000"],
  ["small", 42, "42"],
  ["zero", 0, "0"],
  ["three-digits", 999, "999"],
  ["negative", -1234567, "-1,234,567"],
  ["negative-small", -999, "-999"],
  ["negative-thousand", -1000, "-1,000"],
  ["int64-max", 9223372036854775807n, "9,223,372,036,854,775,807"],
  ["int64-min", -9223372036854775808n, "-9,223,372,036,854,775,808"],
  ["six-digits", 100000, "100,000"],
  ["seven-digits", 1000000, "1,000,000"],
];

for (const [name, n, want] of cases) {
  test(name, () => {
    assert.equal(groupThousands(n), want);
  });
}
