// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

const MIN = -(2n ** 63n);
const MAX = 2n ** 63n - 1n;
const LIMIT = 2n ** 63n;

// digitValue returns the value of one ASCII digit or letter, or -1. It does
// not use a regular expression with the i and u flags, because that matches
// the Kelvin sign U+212A as the letter k.
function digitValue(c) {
  const code = c.charCodeAt(0);
  if (code >= 48 && code <= 57) return code - 48; // 0 to 9
  if (code >= 97 && code <= 122) return code - 87; // a to z
  if (code >= 65 && code <= 90) return code - 55; // A to Z
  return -1;
}

/**
 * Parses s as a signed 64-bit integer in the given base, from 2 to 36, and
 * returns a bigint. Throws a SyntaxError when s is not an optional sign and
 * ASCII digits of the base, and a RangeError when the value does not fit.
 */
export function parseInteger(s, base = 10) {
  const negative = s.startsWith("-");
  const digits = negative || s.startsWith("+") ? s.slice(1) : s;
  if (digits === "") {
    throw new SyntaxError(`invalid integer for base ${base}: ${JSON.stringify(s)}`);
  }
  const radix = BigInt(base);
  let value = 0n;
  for (const c of digits) {
    const d = digitValue(c);
    if (d < 0 || d >= base) {
      throw new SyntaxError(`invalid integer for base ${base}: ${JSON.stringify(s)}`);
    }
    // Stop once the value is above 2^63, so a long string of digits does
    // not build a huge bigint. The value stays above the range.
    if (value <= LIMIT) value = value * radix + BigInt(d);
  }
  if (negative) value = -value;
  if (value < MIN || value > MAX) {
    throw new RangeError(`integer out of range: ${JSON.stringify(s)}`);
  }
  return value;
}
