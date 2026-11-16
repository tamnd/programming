// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { compose, composeAll } from "./compose.ts";

const times = (names: string[], n: number): string[] =>
  Array.from({ length: n }, () => names).flat();

const cases: [string, string[], number, number][] = [
  ["inc-after-double", ["inc", "double"], 3, 7],
  ["double-after-inc", ["double", "inc"], 3, 8],
  ["three-functions", ["negate", "dec", "double"], 5, -9],
  ["empty", [], 42, 42],
  ["single-zero", ["negate"], 0, 0],
  ["negative", ["dec", "negate"], -5, 4],
  ["low", ["inc"], -1000, -999],
  ["duplicates", ["inc", "inc", "inc"], 7, 10],
  ["limit-high", times(["double"], 20), 1000, 1048576000],
  ["limit-low", times(["double"], 20), -1000, -1048576000],
  ["mixed-20", times(["double", "inc"], 10), 1000, 1026046],
  ["mixed-20-swapped", times(["inc", "double"], 10), 1000, 1025023],
];

for (const [name, ops, x, want] of cases) {
  test(name, () => {
    assert.equal(composeAll(ops, x), want);
  });
}

test("compose is generic", () => {
  const shout = compose(
    (s: string) => s + "!",
    (s: string) => s.toUpperCase(),
    (s: string) => s.trim(),
  );
  assert.equal(shout("  hello "), "HELLO!");
});

test("compose of nothing is the identity", () => {
  assert.equal(compose<string>()("same"), "same");
});
