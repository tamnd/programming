// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

export function grid(rows: number, cols: number, value: number): number[][] {
  // The function makes a new row for each row. new Array(rows).fill(row)
  // would put one row object in every slot.
  return Array.from({ length: rows }, () => new Array<number>(cols).fill(value));
}
