// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { collatzAfter, collatzStep, repeat } from "./repeat.js";

const cases = [
  ["one-step", 6, 1, 3],
  ["three-steps", 6, 3, 5],
  ["ten-steps", 27, 10, 214],
  ["zero-times", 7, 0, 7],
  ["start-one", 1, 1, 4],
  ["cycle", 1, 3, 1],
  ["past-one", 16, 10, 1],
  ["peak-27", 27, 77, 9232],
  ["peak-9663", 9663, 48, 27114424],
  ["limit", 10000, 1000, 2],
];

for (const [name, start, times, want] of cases) {
  test(name, () => {
    assert.equal(collatzAfter(start, times), want);
  });
}

test("repeat takes an arrow function", () => {
  assert.equal(repeat((x) => x * 2, 10)(1), 1024);
});

test("repeat with zero times is the identity", () => {
  assert.equal(repeat(collatzStep, 0)(27), 27);
});
