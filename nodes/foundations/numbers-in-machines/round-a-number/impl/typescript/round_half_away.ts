// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.

// Math.round rounds a half toward +Infinity, so Math.round(-2.5) is -2.
// Round the magnitude, then set the sign. Adding 0 turns -0 into 0.
export function roundHalfAway(x: number): number {
  return Math.sign(x) * Math.round(Math.abs(x)) + 0;
}
