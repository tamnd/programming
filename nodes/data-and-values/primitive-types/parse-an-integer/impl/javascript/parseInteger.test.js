// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { parseInteger } from "./parseInteger.js";

const cases = [
  ["decimal", "42", 10, 42n],
  ["negative", "-17", 10, -17n],
  ["hex", "ff", 16, 255n],
  ["binary", "101010", 2, 42n],
  ["base-36", "zz", 36, 1295n],
  ["plus-sign", "+5", 10, 5n],
  ["negative-zero", "-0", 10, 0n],
  ["zero", "0", 10, 0n],
  ["leading-zeros", "007", 10, 7n],
  ["many-leading-zeros", "0".repeat(5000) + "42", 10, 42n],
  ["max", "9223372036854775807", 10, 9223372036854775807n],
  ["min", "-9223372036854775808", 10, -9223372036854775808n],
  ["max-hex", "7fffffffffffffff", 16, 9223372036854775807n],
  ["min-binary", "-1" + "0".repeat(63), 2, -9223372036854775808n],
  ["upper-case", "FF", 16, 255n],
  ["mixed-case", "Zz", 36, 1295n],
  ["above-safe-integer", "9007199254740993", 10, 9007199254740993n],
];

const invalid = [
  ["empty", "", 10],
  ["sign-only", "-", 10],
  ["plus-only", "+", 10],
  ["double-sign", "+-5", 10],
  ["leading-space", " 42", 10],
  ["trailing-space", "42 ", 10],
  ["trailing-newline", "42\n", 10],
  ["trailing-garbage", "42px", 10],
  ["underscore", "1_000", 10],
  ["thousands-separator", "1,000", 10],
  ["decimal-point", "4.0", 10],
  ["exponent", "1e3", 10],
  ["hex-prefix", "0x1A", 16],
  ["binary-prefix", "0b101", 2],
  ["digit-outside-base", "102", 2],
  ["letter-outside-base", "fg", 16],
  ["arabic-indic-digits", "\u0661\u0662", 10],
  ["fullwidth-digits", "\uff11\uff12", 10],
  ["kelvin-sign", "\u212a", 36],
];

const outOfRange = [
  ["max-plus-one", "9223372036854775808", 10],
  ["min-minus-one", "-9223372036854775809", 10],
  ["two-to-the-64", "18446744073709551616", 10],
  ["hex-sign-bit", "8000000000000000", 16],
  ["hex-all-ones", "ffffffffffffffff", 16],
  ["min-times-ten", "-92233720368547758080", 10],
  ["many-digits", "9".repeat(5000), 10],
];

for (const [name, s, base, want] of cases) {
  test(name, () => {
    assert.equal(parseInteger(s, base), want);
  });
}

for (const [name, s, base] of invalid) {
  test(name, () => {
    assert.throws(() => parseInteger(s, base), SyntaxError);
  });
}

for (const [name, s, base] of outOfRange) {
  test(name, () => {
    assert.throws(() => parseInteger(s, base), RangeError);
  });
}
