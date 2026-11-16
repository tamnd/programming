// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

const TABLE = [
  [1000, "M"], [900, "CM"], [500, "D"], [400, "CD"],
  [100, "C"], [90, "XC"], [50, "L"], [40, "XL"],
  [10, "X"], [9, "IX"], [5, "V"], [4, "IV"], [1, "I"],
];
const VALUES = new Map([
  ["I", 1], ["V", 5], ["X", 10], ["L", 50], ["C", 100], ["D", 500], ["M", 1000],
]);

/** Returns the Roman numeral of n, for 1 <= n <= 3999. */
export function toRoman(n) {
  if (!Number.isInteger(n) || n < 1 || n > 3999) {
    throw new RangeError("n is out of range");
  }
  let s = "";
  for (const [value, symbol] of TABLE) {
    while (n >= value) {
      s += symbol;
      n -= value;
    }
  }
  return s;
}

/** Returns the value of a Roman numeral in the standard form. */
export function fromRoman(s) {
  if (s.length === 0) {
    throw new SyntaxError("not a Roman numeral");
  }
  let total = 0;
  for (let i = 0; i < s.length; i++) {
    const value = VALUES.get(s[i]);
    if (value === undefined) {
      throw new SyntaxError("not a Roman numeral");
    }
    const next = i + 1 < s.length ? (VALUES.get(s[i + 1]) ?? 0) : 0;
    total += value < next ? -value : value;
  }
  if (total < 1 || total > 3999 || toRoman(total) !== s) {
    throw new SyntaxError("not in the standard form");
  }
  return total;
}
