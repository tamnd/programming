// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

export function chunks(items: readonly number[], size: number): number[][] {
  const out: number[][] = [];
  for (let i = 0; i < items.length; i += size) {
    // slice makes a copy and stops at the end of the array.
    out.push(items.slice(i, i + size));
  }
  return out;
}
