// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

/**
 * Returns the sum of values. The annotation : number is optional, because
 * the compiler infers the type from the initial value 0.
 */
export function total(values: readonly number[]): number {
  let result: number = 0;
  for (const value of values) {
    result = result + value;
  }
  return result;
}
