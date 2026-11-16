// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

/**
 * Returns "Order {name}: count item(s), total count*price". A single brace
 * needs no escape in a template literal. The total has 2 decimals.
 */
export function describeOrder(name: string, count: number, price: number): string {
  return `Order {${name}}: ${count} item${count === 1 ? "" : "s"}, total ${(count * price).toFixed(2)}`;
}
