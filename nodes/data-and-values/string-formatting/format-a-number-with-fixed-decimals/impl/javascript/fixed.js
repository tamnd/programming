// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

/**
 * Returns x with exactly digits digits after the decimal point. toFixed
 * rounds the exact binary value of x. It uses an exponent for 1e21 and more.
 */
export function fixed(x, digits) {
  return x.toFixed(digits);
}
