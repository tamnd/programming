// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

import assert from "node:assert/strict";
import { test } from "node:test";
import { inspect } from "node:util";

import { Color, formatColors } from "./color.js";

const cases = [
  ["orange", [[255], [128], [0]], ["#FF8000"]],
  ["three", [[255, 0, 0], [0, 255, 0], [0, 0, 255]], ["#FF0000", "#00FF00", "#0000FF"]],
  ["mixed", [[18, 171], [52, 205], [86, 239]], ["#123456", "#ABCDEF"]],
  ["empty", [[], [], []], []],
  ["single", [[1], [2], [3]], ["#010203"]],
  ["zero", [[0], [0], [0]], ["#000000"]],
  ["max", [[255], [255], [255]], ["#FFFFFF"]],
  ["padding", [[10, 0], [11, 15], [12, 16]], ["#0A0B0C", "#000F10"]],
];

for (const [name, args, want] of cases) {
  test(name, () => {
    assert.deepEqual(formatColors(...args), want);
  });
}

test("forms", () => {
  const c = new Color(255, 128, 0);
  assert.equal(String(c), "#FF8000");
  assert.equal(`${c}`, "#FF8000");
  assert.equal("color " + c, "color #FF8000");
  assert.equal(inspect(c), "Color(r=255, g=128, b=0)");
});

test("bad value", () => {
  assert.throws(() => new Color(256, 0, 0), RangeError);
});
