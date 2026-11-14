// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { addToDate, daysInMonth } from "./add.js";

const cases = [
  ["one-day", [2024, 3, 10, 0, 0, 1], "2024-03-11"],
  ["one-month", [2024, 1, 15, 0, 1, 0], "2024-02-15"],
  ["one-year", [2023, 6, 1, 1, 0, 0], "2024-06-01"],
  ["day-into-next-year", [2023, 12, 31, 0, 0, 1], "2024-01-01"],
  ["nothing-to-add", [2024, 2, 29, 0, 0, 0], "2024-02-29"],
  ["month-end-to-leap-february", [2024, 1, 31, 0, 1, 0], "2024-02-29"],
  ["month-end-to-short-february", [2023, 1, 31, 0, 1, 0], "2023-02-28"],
  ["month-end-to-30-day-month", [2024, 3, 31, 0, 1, 0], "2024-04-30"],
  ["back-from-month-end", [2024, 5, 31, 0, -1, 0], "2024-04-30"],
  ["back-to-leap-february", [2024, 3, 31, 0, -1, 0], "2024-02-29"],
  ["leap-day-plus-one-year", [2024, 2, 29, 1, 0, 0], "2025-02-28"],
  ["leap-day-plus-four-years", [2024, 2, 29, 4, 0, 0], "2028-02-29"],
  ["year-and-month-at-once", [2024, 2, 29, 1, 1, 0], "2025-03-29"],
  ["months-before-days", [2024, 1, 31, 0, 1, 1], "2024-03-01"],
  ["month-into-next-year", [2023, 12, 15, 0, 1, 0], "2024-01-15"],
  ["thirteen-months-back", [2024, 1, 15, 0, -13, 0], "2022-12-15"],
  ["day-back-into-last-year", [2024, 1, 1, 0, 0, -1], "2023-12-31"],
  ["year-1900-is-not-leap", [1900, 2, 28, 0, 0, 1], "1900-03-01"],
  ["year-2100-is-not-leap", [2096, 2, 29, 4, 0, 0], "2100-02-28"],
  ["many-days", [1970, 1, 1, 0, 0, 2932896], "9999-12-31"],
  ["many-days-back", [9999, 12, 31, 0, 0, -3652058], "0001-01-01"],
  ["many-months", [1, 1, 31, 0, 119987, 0], "9999-12-31"],
];

for (const [name, fields, want] of cases) {
  test(name, () => {
    const [year, month, day, years, months, days] = fields;
    assert.equal(addToDate(year, month, day, years, months, days), want);
  });
}

test("daysInMonth", () => {
  assert.equal(daysInMonth(2024, 2), 29);
  assert.equal(daysInMonth(1900, 2), 28);
  assert.equal(daysInMonth(2024, 12), 31);
});

test("setUTCMonth alone moves past the end of the month", () => {
  const date = new Date(Date.UTC(2024, 0, 31));
  date.setUTCMonth(1);
  assert.equal(date.toISOString().slice(0, 10), "2024-03-02");
});
