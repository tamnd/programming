// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

export function halvings(n: number): number[] {
  const out: number[] = [];
  while (n > 0) {
    out.push(n);
    // n / 2 can have a fraction, so drop it.
    n = Math.trunc(n / 2);
  }
  return out;
}
