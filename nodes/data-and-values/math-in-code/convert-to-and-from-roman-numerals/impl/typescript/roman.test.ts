// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { fromRoman, toRoman } from "./roman.ts";

const cases: [string, number, string][] = [
  ["year-1990", 1990, "MCMXC"],
  ["year-2008", 2008, "MMVIII"],
  ["each-symbol-once", 1666, "MDCLXVI"],
  ["one", 1, "I"],
  ["four", 4, "IV"],
  ["nine", 9, "IX"],
  ["forty-nine", 49, "XLIX"],
  ["ninety-nine", 99, "XCIX"],
  ["four-hundred-forty-four", 444, "CDXLIV"],
  ["year-1994", 1994, "MCMXCIV"],
  ["longest", 3888, "MMMDCCCLXXXVIII"],
  ["largest", 3999, "MMMCMXCIX"],
];

for (const [name, n, want] of cases) {
  test(name, () => {
    assert.equal(toRoman(n), want);
    assert.equal(fromRoman(want), n);
  });
}

test("round trip", () => {
  for (let n = 1; n <= 3999; n++) {
    assert.equal(fromRoman(toRoman(n)), n);
  }
});

test("invalid", () => {
  for (const s of ["", "IIII", "IC", "VX", "MMMM", "iv", "ABC", "IIV", "XM"]) {
    assert.throws(() => fromRoman(s), SyntaxError, s);
  }
});

test("out of range", () => {
  for (const n of [0, -1, 4000]) {
    assert.throws(() => toRoman(n), RangeError);
  }
});
