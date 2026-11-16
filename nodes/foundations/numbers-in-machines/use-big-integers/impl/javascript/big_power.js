// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

/**
 * Returns base to the power exp as decimal text. A BigInt has no fixed width,
 * so the result is exact; a number would lose digits above 2 ** 53.
 * @param {number} base
 * @param {number} exp
 * @returns {string}
 */
export function bigPower(base, exp) {
  return (BigInt(base) ** BigInt(exp)).toString();
}
