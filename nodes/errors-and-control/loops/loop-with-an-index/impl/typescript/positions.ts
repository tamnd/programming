// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

export function positions(items: readonly string[], target: string): number[] {
  const out: number[] = [];
  // entries() gives [index, item] pairs.
  for (const [i, x] of items.entries()) {
    if (x === target) {
      out.push(i);
    }
  }
  return out;
}
