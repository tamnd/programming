// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

/**
 * Returns one line of a table: name in 10 columns on the left, qty in 5 on
 * the right and price with 2 decimals in 8 on the right. A width is a
 * minimum, so a long value is not cut.
 */
export function formatRow(name, qty, price) {
  return `${name.padEnd(10)}|${String(qty).padStart(5)}|${price.toFixed(2).padStart(8)}`;
}
