// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

/** Returns the characters of a followed by the characters of b. */
export function concat(a, b) {
  return a + b;
}

/** Returns s followed by the decimal form of n. */
export function concatNumber(s, n) {
  // A template literal converts n to a string. With +, the result depends on
  // the order of the operands (see Pitfalls).
  return `${s}${n}`;
}
