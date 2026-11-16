// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { constructionCount, idGenerator, singletonReport } from "./id-generator.ts";
import * as again from "./id-generator.ts";

const cases: [string, number, number[]][] = [
  ["three-calls", 3, [2, 1, 1]],
  ["ten-calls", 10, [9, 1, 1]],
  ["single-call", 1, [0, 1, 1]],
  ["pair", 2, [1, 1, 1]],
  ["limit", 1000000, [999999, 1, 1]],
];

for (const [name, calls, want] of cases) {
  test(name, () => {
    assert.deepEqual(singletonReport(calls), want);
  });
}

test("a second import gets the same instance", () => {
  assert.equal(again.idGenerator, idGenerator);
  assert.equal(constructionCount(), 1);
});
