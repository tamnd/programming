// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

export function filled(n: number, value: number): number[] {
  // Array.from calls the function once for each index, so no slot is empty.
  return Array.from({ length: n }, () => value);
}
