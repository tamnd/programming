// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

/** Returns the characters of a followed by the characters of b. */
export function concat(a: string, b: string): string {
  return a + b;
}

/** Returns s followed by the decimal form of n. */
export function concatNumber(s: string, n: number): string {
  // A template literal converts n to a string. With +, the result depends on
  // the order of the operands (see Pitfalls).
  return `${s}${n}`;
}
