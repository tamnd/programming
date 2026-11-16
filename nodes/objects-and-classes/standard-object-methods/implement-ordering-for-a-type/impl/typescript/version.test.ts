// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { Version, sortVersions } from "./version.ts";

const cases: [string, string[], string[]][] = [
  ["simple", ["1.2.0", "1.0.0", "1.1.0"], ["1.0.0", "1.1.0", "1.2.0"]],
  ["fields-in-order", ["2.0.0", "1.9.9", "1.10.0"], ["1.9.9", "1.10.0", "2.0.0"]],
  ["patch", ["0.1.10", "0.1.2", "0.1.1"], ["0.1.1", "0.1.2", "0.1.10"]],
  ["empty", [], []],
  ["single", ["3.4.5"], ["3.4.5"]],
  ["numeric-not-text", ["10.0.0", "9.0.0", "1.0.0"], ["1.0.0", "9.0.0", "10.0.0"]],
  ["duplicates", ["1.0.0", "0.9.0", "1.0.0"], ["0.9.0", "1.0.0", "1.0.0"]],
  ["zero", ["0.0.1", "0.0.0"], ["0.0.0", "0.0.1"]],
  ["limit", ["1000000.0.0", "999999.1000000.1000000"], ["999999.1000000.1000000", "1000000.0.0"]],
];

for (const [name, versions, want] of cases) {
  test(name, () => {
    assert.deepEqual(sortVersions(versions), want);
  });
}

test("order rules", () => {
  const a = new Version(1, 2, 3);
  const b = new Version(1, 10, 0);
  assert.ok(a.compareTo(b) < 0 && b.compareTo(a) > 0);
  assert.equal(a.compareTo(new Version(1, 2, 3)), 0);
  assert.ok(a.equals(new Version(1, 2, 3)) && !a.equals(b));
  assert.deepEqual(["1.10.0", "1.2.0"].sort(), ["1.10.0", "1.2.0"]);
});
