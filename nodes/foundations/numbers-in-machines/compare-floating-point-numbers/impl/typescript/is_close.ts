// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

// Tells whether a and b are close, by the rule of math.isclose in Python:
// |a - b| <= max(relTol * max(|a|, |b|), absTol). NaN is close to no value,
// and an infinity is close only to itself.
export function isClose(a: number, b: number, relTol: number, absTol: number): boolean {
  if (a === b) return true; // also Infinity === Infinity
  if (!Number.isFinite(a) || !Number.isFinite(b)) return false; // NaN or an infinity
  const diff = Math.abs(a - b);
  return diff <= Math.max(relTol * Math.max(Math.abs(a), Math.abs(b)), absTol);
}
