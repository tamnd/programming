// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

// TypeScript has no complex type, so a complex number is a pair [re, im].

/** A complex number as [real part, imaginary part]. */
export type Complex = readonly [number, number];

/** Returns the sum of two complex numbers. */
export function add([a, b]: Complex, [c, d]: Complex): [number, number] {
  return [a + c, b + d];
}

/** Returns the product of two complex numbers. */
export function mul([a, b]: Complex, [c, d]: Complex): [number, number] {
  return [a * c - b * d, a * d + b * c];
}

/**
 * Returns the quotient of two complex numbers. It divides by the larger part
 * of the divisor first, so c * c + d * d is never computed and cannot overflow.
 */
export function div([a, b]: Complex, [c, d]: Complex): [number, number] {
  if (Math.abs(c) >= Math.abs(d)) {
    const r = d / c;
    const t = c + d * r;
    return [(a + b * r) / t, (b - a * r) / t];
  }
  const r = c / d;
  const t = c * r + d;
  return [(a * r + b) / t, (b * r - a) / t];
}

/** Returns [sum, product, quotient, abs(a), phase(a)] as 8 numbers. */
export function complexArithmetic(a: Complex, b: Complex): number[] {
  return [...add(a, b), ...mul(a, b), ...div(a, b), Math.hypot(a[0], a[1]), Math.atan2(a[1], a[0])];
}
