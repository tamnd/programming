// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

export function rotateLeft(items: readonly number[], k: number): number[] {
  if (items.length === 0) {
    return [];
  }
  // k % items.length makes a k that is larger than the array work.
  const s = k % items.length;
  return [...items.slice(s), ...items.slice(0, s)];
}
