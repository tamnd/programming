// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { numberLine } from "./numberLine.js";

const cases = [
  ["zero", 0, ""],
  ["one", 1, "1"],
  ["three", 3, "1,2,3"],
  ["five", 5, "1,2,3,4,5"],
  ["seven", 7, "1,2,3,4,5,6,7"],
  ["nine", 9, "1,2,3,4,5,6,7,8,9"],
  ["ten", 10, "1,2,3,4,5,6,7,8,9,10"],
];

// Long results: the length, the start and the end.
const long = [
  ["hundred", 100, 291, ",99,100"],
  ["hundred-and-one", 101, 295, ",100,101"],
  ["thousand", 1000, 3892, ",999,1000"],
  ["large", 100000, 588894, ",99999,100000"],
];

for (const [name, n, want] of cases) {
  test(name, () => {
    assert.equal(numberLine(n), want);
  });
}

for (const [name, n, size, end] of long) {
  test(name, () => {
    const got = numberLine(n);
    assert.equal(got.length, size);
    assert.ok(got.startsWith("1,2,3,"));
    assert.ok(got.endsWith(end));
  });
}
