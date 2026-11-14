// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

import assert from "node:assert/strict";
import { hrtime } from "node:process";
import { test } from "node:test";

import { sleepFor, sleepRequests } from "./sleep.js";

const cases = [
  ["one-call", 1000000n, 0n, [1000000n], [1000000n]],
  ["woke-late", 1000000n, 0n, [1080000n], [1000000n]],
  ["woke-early", 1000000n, 0n, [600000n, 1000000n], [1000000n, 400000n]],
  ["one-second", 1000000000n, 5000000000n, [6000050000n], [1000000000n]],
  ["zero", 0n, 0n, [], []],
  ["negative", -5000000n, 0n, [], []],
  ["most-negative", -4611686018427387904n, 0n, [], []],
  ["several-early-wakes", 10000000n, 0n, [2000000n, 5000000n, 9999999n, 10000000n], [10000000n, 8000000n, 5000000n, 1n]],
  ["no-progress", 1000000n, 0n, [0n, 1000000n], [1000000n, 1000000n]],
  ["exact-deadline", 3000000n, 100n, [3000100n], [3000000n]],
  ["one-ns-short", 3000000n, 0n, [2999999n, 3000000n], [3000000n, 1n]],
  ["extra-readings", 1000000n, 0n, [1000000n, 5000000n], [1000000n]],
  ["negative-origin", 2000000000n, -5000000000n, [-4000000000n, -3000000000n], [2000000000n, 1000000000n]],
  ["origin-after-100-days", 1500000n, 8640000000000000n, [8640000001000000n, 8640000001500000n], [1500000n, 500000n]],
  ["one-nanosecond", 1n, 0n, [1000000n], [1n]],
  ["sub-millisecond", 1500000n, 0n, [1000000n, 1500000n], [1500000n, 500000n]],
  ["one-hour", 3600000000000n, 0n, [3600000000000n], [3600000000000n]],
  ["thirty-days", 2592000000000000n, 0n, [2147483647000000n, 2592000000000000n], [2592000000000000n, 444516353000000n]],
];

for (const [name, durationNs, startNs, readings, want] of cases) {
  test(name, async () => {
    assert.deepEqual(await sleepRequests(durationNs, startNs, readings), want);
  });
}

test("sleepFor waits at least the duration", async () => {
  const start = hrtime.bigint();
  await sleepFor(20_000_000n);
  assert.ok(hrtime.bigint() - start >= 20_000_000n);
});

test("sleepFor returns at once for zero and a negative duration", async () => {
  for (const ns of [0n, -5_000_000n]) {
    const start = hrtime.bigint();
    await sleepFor(ns);
    assert.ok(hrtime.bigint() - start < 1_000_000_000n);
  }
});

test("two sleeps at the same time take the time of one", async () => {
  const start = hrtime.bigint();
  await Promise.all([sleepFor(100_000_000n), sleepFor(100_000_000n)]);
  const elapsed = hrtime.bigint() - start;
  assert.ok(elapsed >= 100_000_000n && elapsed < 200_000_000n);
});
