// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

/**
 * Returns start, start + step, ... while the value has not passed stop.
 * @param {number} start
 * @param {number} stop
 * @param {number} step a number that is not 0
 * @returns {number[]}
 */
export function countBy(start, stop, step) {
  const out = [];
  // The test depends on the direction of the step.
  for (let i = start; step > 0 ? i <= stop : i >= stop; i += step) {
    out.push(i);
  }
  return out;
}
