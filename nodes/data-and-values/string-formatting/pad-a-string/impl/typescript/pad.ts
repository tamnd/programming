// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

/**
 * Returns s padded with fill to width, once on the left and once on the
 * right. If s has width characters or more, both results are s.
 */
export function pad(s: string, width: number, fill: string): [string, string] {
  return [s.padStart(width, fill), s.padEnd(width, fill)];
}
