// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

// repeat returns n copies of s. String.prototype.repeat throws a RangeError
// if n is negative.
export function repeat(s: string, n: number): string {
  return s.repeat(n);
}
