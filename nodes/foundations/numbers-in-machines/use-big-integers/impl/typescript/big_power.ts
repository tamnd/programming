// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

// Returns base to the power exp as decimal text. A bigint has no fixed width,
// so the result is exact; a number would lose digits above 2 ** 53.
export function bigPower(base: number, exp: number): string {
  return (BigInt(base) ** BigInt(exp)).toString();
}
