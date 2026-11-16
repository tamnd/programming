// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

/**
 * Returns n in decimal with a comma between each group of 3 digits. n can
 * be a number or a bigint, so a 64-bit value keeps all its digits.
 */
export function groupThousands(n) {
  const s = String(n);
  const sign = s.startsWith("-") ? "-" : "";
  const digits = s.slice(sign.length);
  let out = sign;
  for (let i = 0; i < digits.length; i++) {
    if (i > 0 && (digits.length - i) % 3 === 0) {
      out += ",";
    }
    out += digits[i];
  }
  return out;
}
