// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { nowUnixMillis, nowUnixSeconds, unixTimestamp } from "./unix.ts";

const cases: [string, number[], number][] = [
  ["epoch", [1970, 1, 1, 0, 0, 0, 0], 0],
  ["billennium", [2001, 9, 9, 1, 46, 40, 0], 1000000000],
  ["negative-offset", [2024, 3, 10, 2, 0, 0, -300], 1710054000],
  ["positive-offset", [2024, 3, 10, 16, 0, 0, 540], 1710054000],
  ["one-second-before-epoch", [1969, 12, 31, 23, 59, 59, 0], -1],
  ["offset-moves-before-epoch", [1970, 1, 1, 0, 0, 0, 60], -3600],
  ["offset-moves-to-next-year", [2023, 12, 31, 19, 0, 0, -300], 1704067200],
  ["offset-of-45-minutes", [2024, 1, 1, 5, 45, 0, 345], 1704067200],
  ["offset-of-minus-9-30", [2023, 12, 31, 14, 30, 0, -570], 1704067200],
  ["offset-plus-14", [1970, 1, 1, 14, 0, 0, 840], 0],
  ["offset-minus-12", [1969, 12, 31, 12, 0, 0, -720], 0],
  ["leap-day", [2024, 2, 29, 0, 0, 0, 0], 1709164800],
  ["year-2000-is-leap", [2000, 2, 29, 0, 0, 0, 0], 951782400],
  ["year-1900-is-not-leap", [1900, 3, 1, 0, 0, 0, 0], -2203891200],
  ["int32-max", [2038, 1, 19, 3, 14, 7, 0], 2147483647],
  ["after-int32-max", [2038, 1, 19, 3, 14, 8, 0], 2147483648],
  ["int32-min", [1901, 12, 13, 20, 45, 52, 0], -2147483648],
  ["year-99-is-not-1999", [99, 12, 31, 23, 59, 59, 0], -59011459201],
  ["first-second-of-year-1", [1, 1, 1, 0, 0, 0, 0], -62135596800],
  ["last-second-of-year-9999", [9999, 12, 31, 23, 59, 59, 0], 253402300799],
];

for (const [name, fields, want] of cases) {
  test(name, () => {
    const [year, month, day, hour, minute, second, offset] = fields;
    assert.equal(unixTimestamp(year, month, day, hour, minute, second, offset), want);
  });
}

test("Date.UTC reads year 99 as 1999", () => {
  assert.equal(Date.UTC(99, 11, 31, 23, 59, 59) / 1000, 946684799);
  assert.equal(unixTimestamp(99, 12, 31, 23, 59, 59, 0), -59011459201);
});

test("back to a date", () => {
  assert.equal(new Date(1710054000 * 1000).toISOString(), "2024-03-10T07:00:00.000Z");
  assert.equal(new Date(-62135596800 * 1000).toISOString(), "0001-01-01T00:00:00.000Z");
});

test("now", () => {
  const seconds = nowUnixSeconds();
  const millis = nowUnixMillis();
  assert.ok(Number.isInteger(seconds) && seconds >= 1704067200);
  const diff = Math.floor(millis / 1000) - seconds;
  assert.ok(diff >= 0 && diff <= 1);
});
