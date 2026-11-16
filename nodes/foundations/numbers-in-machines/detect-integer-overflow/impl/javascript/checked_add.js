// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

// A number holds integers exactly only up to 2 ** 53 - 1, so 64-bit values
// are BigInt values. BigInt arithmetic is exact. BigInt.asIntN(64, s) keeps
// the low 64 bits of s as a signed value, so it changes s only when s does not
// fit in 64 bits.
export function checkedAdd(a, b) {
  const s = a + b;
  return BigInt.asIntN(64, s) === s ? s : null;
}
