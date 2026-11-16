// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { bump, countPerThread, current } from "./tls.ts";

const cases: [string, number[]][] = [
  ["three-threads", [1, 2, 3]],
  ["uneven", [10, 0, 5, 1000]],
  ["empty", []],
  ["zero", [0]],
  ["one-thread", [7]],
  ["equal-counts", [100, 100, 100, 100]],
  ["many-threads", Array.from({ length: 16 }, (_, i) => i + 1)],
  ["large", [100000, 100000]],
];

for (const [name, counts] of cases) {
  test(name, async () => {
    assert.deepEqual(await countPerThread(counts), counts);
  });
}

test("the counter of the main thread does not change", async () => {
  const before = current();
  for (let k = 0; k < 3; k++) bump();
  await countPerThread([5, 6]);
  assert.equal(current(), before + 3);
});
