// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

/** A matrix as a list of rows. */
export type Matrix = readonly (readonly number[])[];

/** Returns the transpose of m: row i of the result is column i of m. */
export function transpose(m: Matrix): number[][] {
  return m[0]!.map((_, j) => m.map((row) => row[j]!));
}

/**
 * Returns the product a times b. The number of columns of a must be the
 * number of rows of b. Neither matrix changes.
 */
export function matrixMultiply(a: Matrix, b: Matrix): number[][] {
  if (a[0]!.length !== b.length) {
    throw new RangeError(`${a.length}x${a[0]!.length} times ${b.length}x${b[0]!.length}: the sizes do not match`);
  }
  return a.map((row) => b[0]!.map((_, j) => row.reduce((sum, x, k) => sum + x * b[k]![j]!, 0)));
}
