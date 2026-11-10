// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { findDates, reformatDates } from "./reformatDates.js";

const cases = [
  ["one-date", "Due 2024-01-02.", "-", "/", "Due 02/01/2024."],
  ["two-dates", "From 2024-01-02 to 2024-12-31", "-", "/", "From 02/01/2024 to 31/12/2024"],
  ["no-date", "no dates here", "-", "/", "no dates here"],
  ["empty-text", "", "-", "/", ""],
  ["empty-to-sep", "2024-01-02", "-", "", "02012024"],
  ["whole-text", "2024-01-02", "-", "/", "02/01/2024"],
  ["adjacent", "2024-01-02,2024-01-03", "-", "/", "02/01/2024,03/01/2024"],
  ["digit-before", "12024-01-02", "-", "/", "12024-01-02"],
  ["digit-after", "2024-01-023", "-", "/", "2024-01-023"],
  ["date-after-rejected", "2024-01-0234-05-06", "-", "/", "2024-01-06/05/0234"],
  ["chained", "2024-01-02-03", "-", "/", "02/01/2024-03"],
  ["other-separator", "2024/01/02", "-", ".", "2024/01/02"],
  ["dot-separator", "2024x01x02 2024.01.02", ".", "-", "2024x01x02 02-01-2024"],
  ["backslash-separator", "2024\\01\\02", "\\", "-", "02-01-2024"],
  ["plus-separator", "2024+01+02 202401102", "+", "-", "02-01-2024 202401102"],
  ["dollar-in-replacement", "2024-01-02", "-", "$1", "02$101$12024"],
  ["backslash-in-replacement", "2024-01-02", "-", "\\1\\", "02\\1\\01\\1\\2024"],
  ["digit-in-replacement", "2024-01-02", "-", "0", "0200102024"],
  ["whole-match-in-replacement", "2024-01-02", "-", "$&", "02$&01$&2024"],
  ["unicode-digits", "٢٠٢٤-٠١-٠٢ and ２０２４-01-02 and 2024-01-02", "-", "/", "٢٠٢٤-٠١-٠٢ and ２０２４-01-02 and 02/01/2024"],
  ["non-ascii-text", "Café ☕ 2024-01-02 🙂", "-", "/", "Café ☕ 02/01/2024 🙂"],
  ["non-ascii-separator", "2024–01–02", "–", "·", "02·01·2024"],
  ["multiline", "2024-01-02\n2024-01-03\n", "-", "/", "02/01/2024\n03/01/2024\n"],
  ["not-a-calendar-date", "2024-13-45", "-", "/", "45/13/2024"],
];

const findCases = [
  ["two-dates", "From 2024-01-02 to 2024-12-31", "-", [["2024", "01", "02"], ["2024", "12", "31"]]],
  ["none", "no dates here", "-", []],
  ["empty", "", "-", []],
  ["date-after-rejected", "2024-01-0234-05-06", "-", [["0234", "05", "06"]]],
  ["dot-separator", "2024x01x02 2024.01.02", ".", [["2024", "01", "02"]]],
  ["unicode-digits", "٢٠٢٤-٠١-٠٢ 2024-01-02", "-", [["2024", "01", "02"]]],
];

for (const [name, text, fromSep, toSep, want] of cases) {
  test(`reformat ${name}`, () => {
    assert.equal(reformatDates(text, fromSep, toSep), want);
  });
}

for (const [name, text, sep, want] of findCases) {
  test(`find ${name}`, () => {
    assert.deepEqual(findDates(text, sep), want);
  });
}
