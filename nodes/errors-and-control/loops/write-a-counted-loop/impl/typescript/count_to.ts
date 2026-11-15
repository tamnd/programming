// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

export function countTo(n: number): number[] {
  const out: number[] = [];
  for (let i = 1; i <= n; i++) {
    out.push(i);
  }
  return out;
}
