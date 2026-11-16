// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

/**
 * Tells whether a and b are close, by the rule of math.isclose in Python:
 * |a - b| <= max(relTol * max(|a|, |b|), absTol). NaN is close to no value,
 * and an infinity is close only to itself.
 * @param {number} a
 * @param {number} b
 * @param {number} relTol
 * @param {number} absTol
 * @returns {boolean}
 */
export function isClose(a, b, relTol, absTol) {
  if (a === b) return true; // also Infinity === Infinity
  if (!Number.isFinite(a) || !Number.isFinite(b)) return false; // NaN or an infinity
  const diff = Math.abs(a - b);
  return diff <= Math.max(relTol * Math.max(Math.abs(a), Math.abs(b)), absTol);
}
