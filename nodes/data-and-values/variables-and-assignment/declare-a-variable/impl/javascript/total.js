// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

/**
 * Returns the sum of values. let declares a variable that can get a new
 * value; const declares the loop variable, which gets a new binding on each
 * pass.
 */
export function total(values) {
  let result = 0;
  for (const value of values) {
    result = result + value;
  }
  return result;
}
