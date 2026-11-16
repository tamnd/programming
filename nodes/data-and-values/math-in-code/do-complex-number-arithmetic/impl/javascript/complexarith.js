// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

// JavaScript has no complex type, so a complex number is an array [re, im].

/** Returns the sum of two complex numbers [re, im]. */
export function add([a, b], [c, d]) {
  return [a + c, b + d];
}

/** Returns the product of two complex numbers. */
export function mul([a, b], [c, d]) {
  return [a * c - b * d, a * d + b * c];
}

/**
 * Returns the quotient of two complex numbers. It divides by the larger part
 * of the divisor first, so c * c + d * d is never computed and cannot overflow.
 */
export function div([a, b], [c, d]) {
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
export function complexArithmetic(a, b) {
  return [...add(a, b), ...mul(a, b), ...div(a, b), Math.hypot(a[0], a[1]), Math.atan2(a[1], a[0])];
}
