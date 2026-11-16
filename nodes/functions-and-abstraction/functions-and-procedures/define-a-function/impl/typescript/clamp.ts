// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

/** Returns value limited to the range from low to high. */
export function clamp(value: number, low: number, high: number): number {
  if (value < low) {
    return low;
  }
  if (value > high) {
    return high;
  }
  return value;
}
