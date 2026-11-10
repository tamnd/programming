// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { elapsedMs, measure } from "./elapsed.ts";

const cases: [string, bigint, bigint, string][] = [
  ["one-and-a-half-ms", 1000000000n, 1001500000n, "1.500 ms"],
  ["seconds", 0n, 2345678901n, "2345.678 ms"],
  ["under-one-ms", 50000n, 750000n, "0.700 ms"],
  ["zero", 123456789n, 123456789n, "0.000 ms"],
  ["under-one-microsecond", 0n, 999n, "0.000 ms"],
  ["truncate-not-round", 0n, 1999999n, "1.999 ms"],
  ["leading-zero-in-fraction", 0n, 1005000n, "1.005 ms"],
  ["exact-ms", 0n, 7000000n, "7.000 ms"],
  ["one-hour", 0n, 3600000000000n, "3600000.000 ms"],
  ["negative-readings", -5000000n, -3500000n, "1.500 ms"],
  ["across-zero", -1000000n, 500000n, "1.500 ms"],
  ["beyond-2-pow-53", 9007199254740993n, 9007199255741992n, "1.000 ms"],
  ["near-int64-max", 9223372035854775807n, 9223372036854775807n, "1000.000 ms"],
  ["near-int64-min", -9223372036854775808n, -9223372035854775808n, "1000.000 ms"],
];

for (const [name, startNs, endNs, want] of cases) {
  test(name, () => {
    assert.equal(elapsedMs(startNs, endNs), want);
  });
}

test("numbers lose the difference beyond 2^53", () => {
  const start = 9007199254740993n;
  const end = 9007199255741992n;
  assert.equal(end - start, 1000999n);
  assert.equal(Number(end) - Number(start), 1001000);
});

test("measure with a fake clock", () => {
  const calls: string[] = [];
  const readings = [5_000_000n, 6_500_000n];
  const clock = () => {
    calls.push("clock");
    return readings.shift()!;
  };
  assert.equal(
    measure(() => calls.push("work"), clock),
    1_500_000n,
  );
  assert.deepEqual(calls, ["clock", "work", "clock"]);
});

test("measure with the monotonic clock", () => {
  const calls: string[] = [];
  assert.ok(measure(() => calls.push("work")) >= 0n);
  assert.deepEqual(calls, ["work"]);
});
