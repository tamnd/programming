// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

export function zipJoin(lower: readonly string[], upper: readonly string[], digits: readonly number[]): string[] {
  const out: string[] = [];
  // An index past the end of a shorter list would give undefined.
  const n = Math.min(lower.length, upper.length, digits.length);
  for (let i = 0; i < n; i++) {
    out.push(`${lower[i]}${upper[i]}${digits[i]}`);
  }
  return out;
}
