// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { describeOrder } from "./order.js";

const cases = [
  ["three-items", "Ada", 3, 1.5, "Order {Ada}: 3 items, total 4.50"],
  ["one-item", "Grace", 1, 19.99, "Order {Grace}: 1 item, total 19.99"],
  ["many-items", "Linus", 12, 0.25, "Order {Linus}: 12 items, total 3.00"],
  ["zero-items", "Ada", 0, 9.99, "Order {Ada}: 0 items, total 0.00"],
  ["one-item-plural", "Bob", 1, 0.5, "Order {Bob}: 1 item, total 0.50"],
  ["empty-name", "", 2, 1, "Order {}: 2 items, total 2.00"],
  ["braces-in-name", "{x}", 2, 1.25, "Order {{x}}: 2 items, total 2.50"],
  ["percent-in-name", "50% off", 4, 2.5, "Order {50% off}: 4 items, total 10.00"],
  ["dollar-in-name", "${HOME}", 1, 3, "Order {${HOME}}: 1 item, total 3.00"],
  ["product-rounds", "Cy", 3, 19.99, "Order {Cy}: 3 items, total 59.97"],
  ["large-total", "Dee", 1000, 1234.56, "Order {Dee}: 1000 items, total 1234560.00"],
  ["negative-price", "Refund", 2, -4.5, "Order {Refund}: 2 items, total -9.00"],
];

for (const [name, who, count, price, want] of cases) {
  test(name, () => {
    assert.equal(describeOrder(who, count, price), want);
  });
}
