// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { nowUtc, utcTimestamp } from "./now.ts";

const cases: [string, bigint, string][] = [
  ["epoch", 0n, "1970-01-01T00:00:00.000Z"],
  ["billennium", 1000000000000000000n, "2001-09-09T01:46:40.000Z"],
  ["with-milliseconds", 1710054000123456789n, "2024-03-10T07:00:00.123Z"],
  ["leap-day", 1709208000000000000n, "2024-02-29T12:00:00.000Z"],
  ["truncate-not-round", 999999n, "1970-01-01T00:00:00.000Z"],
  ["one-ns-before-epoch", -1n, "1969-12-31T23:59:59.999Z"],
  ["one-ms-before-epoch", -1000000n, "1969-12-31T23:59:59.999Z"],
  ["just-over-one-ms-before-epoch", -1000001n, "1969-12-31T23:59:59.998Z"],
  ["last-ms-of-a-year", 1704067199999999999n, "2023-12-31T23:59:59.999Z"],
  ["year-2000-is-leap", 951782400000000000n, "2000-02-29T00:00:00.000Z"],
  ["year-2100-is-not-leap", 4107542399999999999n, "2100-02-28T23:59:59.999Z"],
  ["after-int32-seconds", 2147483648000000000n, "2038-01-19T03:14:08.000Z"],
  ["int64-max", 9223372036854775807n, "2262-04-11T23:47:16.854Z"],
  ["int64-min", -9223372036854775808n, "1677-09-21T00:12:43.145Z"],
];

for (const [name, epochNs, want] of cases) {
  test(name, () => {
    assert.equal(utcTimestamp(epochNs), want);
  });
}

test("bigint division alone truncates toward zero", () => {
  assert.equal(-1n / 1_000_000n, 0n);
  assert.equal(utcTimestamp(-1n), "1969-12-31T23:59:59.999Z");
});

test("nowUtc with a fake clock", () => {
  const calls: string[] = [];
  const clock = () => {
    calls.push("clock");
    return 1_710_054_000_123_456_789n;
  };
  assert.equal(nowUtc(clock), "2024-03-10T07:00:00.123Z");
  assert.deepEqual(calls, ["clock"]);
});

test("nowUtc with the real clock", () => {
  const got = nowUtc();
  assert.match(got, /^\d{4}-\d\d-\d\dT\d\d:\d\d:\d\d\.\d{3}Z$/);
  assert.ok(Number(got.slice(0, 4)) >= 2024);
});
