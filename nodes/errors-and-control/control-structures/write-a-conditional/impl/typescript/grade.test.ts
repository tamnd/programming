// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { grade } from "./grade.ts";

const cases: [string, number, string][] = [
  ["ninety-five", 95, "A"],
  ["eighty-five", 85, "B"],
  ["seventy-five", 75, "C"],
  ["sixty-five", 65, "D"],
  ["forty", 40, "F"],
  ["hundred", 100, "A"],
  ["zero", 0, "F"],
  ["ninety", 90, "A"],
  ["eighty-nine", 89, "B"],
  ["sixty", 60, "D"],
  ["fifty-nine", 59, "F"],
];

for (const [name, score, want] of cases) {
  test(name, () => {
    assert.equal(grade(score), want);
  });
}
