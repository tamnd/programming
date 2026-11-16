// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

/** Returns [quotient, remainder] of the floor division of a by b. */
export function divMod(a, b) {
  let q = Math.trunc(a / b);
  let r = a % b;
  if (r !== 0 && r < 0 !== b < 0) {
    q -= 1;
    r += b;
  }
  // a / b and a % b can give -0. Adding 0 turns -0 into 0.
  return [q + 0, r + 0];
}
