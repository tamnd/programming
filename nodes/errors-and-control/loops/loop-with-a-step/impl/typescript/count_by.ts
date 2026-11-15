// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

export function countBy(start: number, stop: number, step: number): number[] {
  const out: number[] = [];
  // The test depends on the direction of the step.
  for (let i = start; step > 0 ? i <= stop : i >= stop; i += step) {
    out.push(i);
  }
  return out;
}
