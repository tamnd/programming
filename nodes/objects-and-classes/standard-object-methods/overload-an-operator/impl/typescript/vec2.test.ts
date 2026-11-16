// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { Vec2, vectorSum } from "./vec2.ts";

const cases: [string, [number[], number[]], number[]][] = [
  ["two", [[1, 3], [2, 4]], [4, 6]],
  ["three", [[1, 2, 3], [10, 20, 30]], [6, 60]],
  ["empty", [[], []], [0, 0]],
  ["single", [[5], [-7]], [5, -7]],
  ["negative", [[-1, -2], [-3, -4]], [-3, -7]],
  ["cancel", [[3, -3], [5, -5]], [0, 0]],
  ["limit", [[1e9, 1e9, 1e9], [-1e9, -1e9, -1e9]], [3e9, -3e9]],
];

for (const [name, args, want] of cases) {
  test(name, () => {
    assert.deepEqual(vectorSum(args[0], args[1]), want);
  });
}

test("methods in place of operators", () => {
  const a = new Vec2(1, 2);
  const b = new Vec2(3, 4);
  assert.ok(a.add(b).equals(new Vec2(4, 6)));
  assert.ok(!a.equals(b) && a.equals(new Vec2(1, 2)));
  // == and === compare references, so two equal vectors are not ===.
  assert.ok(a !== new Vec2(1, 2));
  // + converts both objects to strings.
  assert.equal(`${a}${b}`, "[object Object][object Object]");
});
