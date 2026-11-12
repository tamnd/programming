// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

// compare returns -1, 0 or 1 for the code point order of a and b. The < operator
// compares UTF-16 code units, which puts a code point above U+FFFF before
// U+E000 to U+FFFF, so the loop compares code points.
export function compare(a: string, b: string): number {
  let i = 0;
  while (i < a.length && i < b.length) {
    const x = a.codePointAt(i) as number;
    const y = b.codePointAt(i) as number;
    if (x !== y) {
      return x < y ? -1 : 1;
    }
    // The two strings are the same up to here, so one index fits both.
    i += x > 0xffff ? 2 : 1;
  }
  return Math.sign(a.length - b.length);
}

// equals reports whether a and b have the same code points.
export function equals(a: string, b: string): boolean {
  return a === b;
}
