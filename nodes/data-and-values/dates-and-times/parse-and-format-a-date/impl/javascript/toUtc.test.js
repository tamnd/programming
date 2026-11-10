// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { toUtc } from "./toUtc.js";

// [case, input, expected value]
const cases = [
  ["utc", "2026-11-10T12:00:00Z", "2026-11-10T12:00:00.000Z"],
  ["positive-offset", "2026-11-10T19:30:00+07:00", "2026-11-10T12:30:00.000Z"],
  ["negative-offset", "2026-11-10T07:00:00-05:00", "2026-11-10T12:00:00.000Z"],
  ["back-a-day", "2026-11-11T01:00:00+07:00", "2026-11-10T18:00:00.000Z"],
  ["forward-a-day", "2026-11-10T22:00:00-05:00", "2026-11-11T03:00:00.000Z"],
  ["year-end", "2027-01-01T00:30:00+01:00", "2026-12-31T23:30:00.000Z"],
  ["leap-day", "2024-02-29T23:00:00-02:00", "2024-03-01T01:00:00.000Z"],
  ["half-hour-offset", "2026-11-10T17:30:00+05:30", "2026-11-10T12:00:00.000Z"],
  ["quarter-hour-offset", "2026-11-10T17:45:00+05:45", "2026-11-10T12:00:00.000Z"],
  ["unknown-offset", "2026-11-10T12:00:00-00:00", "2026-11-10T12:00:00.000Z"],
  ["fraction-one-digit", "2026-11-10T12:00:00.5Z", "2026-11-10T12:00:00.500Z"],
  ["fraction-three-digits", "2026-11-10T12:00:00.123+01:00", "2026-11-10T11:00:00.123Z"],
  ["before-epoch", "1970-01-01T00:30:00+01:00", "1969-12-31T23:30:00.000Z"],
  ["before-epoch-fraction", "1969-12-31T23:59:59.999Z", "1969-12-31T23:59:59.999Z"],
  ["largest-offset", "2026-11-10T00:00:00+14:00", "2026-11-09T10:00:00.000Z"],
  ["smallest-offset", "2026-11-10T00:00:00-12:00", "2026-11-10T12:00:00.000Z"],
  ["lower-case", "2026-11-10t12:00:00z", "2026-11-10T12:00:00.000Z"],
  ["year-one", "0001-01-01T00:00:00Z", "0001-01-01T00:00:00.000Z"],
  ["year-9999", "9999-12-31T23:59:59.999Z", "9999-12-31T23:59:59.999Z"],
];

// [case, input that is not a valid RFC 3339 date-time]
const errors = [
  ["no-offset", "2026-11-10T12:00:00"],
  ["date-only", "2026-11-10"],
  ["space-separator", "2026-11-10 12:00:00Z"],
  ["no-seconds", "2026-11-10T12:00Z"],
  ["offset-no-colon", "2026-11-10T12:00:00+0700"],
  ["offset-hours-only", "2026-11-10T12:00:00+07"],
  ["not-a-leap-year", "2026-02-29T12:00:00Z"],
  ["april-31", "2026-04-31T12:00:00Z"],
  ["month-13", "2026-13-01T12:00:00Z"],
  ["day-zero", "2026-11-00T12:00:00Z"],
  ["hour-24", "2026-11-10T24:00:00Z"],
  ["minute-60", "2026-11-10T12:60:00Z"],
  ["leap-second", "2016-12-31T23:59:60Z"],
  ["offset-hour-24", "2026-11-10T12:00:00+24:00"],
  ["offset-minute-60", "2026-11-10T12:00:00+07:60"],
  ["comma-fraction", "2026-11-10T12:00:00,5Z"],
  ["trailing-space", "2026-11-10T12:00:00Z "],
  ["empty", ""],
];

for (const [name, timestamp, want] of cases) {
  test(name, () => {
    assert.equal(toUtc(timestamp), want);
  });
}

for (const [name, timestamp] of errors) {
  test(name, () => {
    assert.throws(() => toUtc(timestamp), RangeError);
  });
}
