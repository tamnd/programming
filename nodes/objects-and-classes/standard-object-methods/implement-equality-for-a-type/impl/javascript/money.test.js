// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { Money, countDistinct } from "./money.js";

const cases = [
  ["two-equal", [1250, 1250], ["EUR", "EUR"], 1],
  ["different-amounts", [100, 200, 100], ["USD", "USD", "USD"], 2],
  ["mixed", [500, 500, 500, 700], ["EUR", "USD", "EUR", "EUR"], 3],
  ["empty", [], [], 0],
  ["case", [999, 999, 999], ["gbp", "GBP", "Gbp"], 1],
  ["other-currency", [0, 0, 0], ["EUR", "USD", "JPY"], 3],
  ["negative", [-5, 5, -5], ["EUR", "EUR", "EUR"], 2],
  ["limit", [1e12, -1e12, 1e12], ["usd", "USD", "USD"], 2],
];

for (const [name, amounts, currencies, want] of cases) {
  test(name, () => {
    assert.equal(countDistinct(amounts, currencies), want);
  });
}

test("equality rules", () => {
  const a = new Money(1250, "EUR");
  const b = new Money(1250, "eur");
  assert.ok(a.equals(a));
  assert.ok(a.equals(b) && b.equals(a));
  assert.ok(!a.equals(new Money(1251, "EUR")));
  assert.ok(!a.equals(new Money(1250, "USD")));
  assert.ok(!a.equals(null));
  assert.ok(!a.equals({ cents: 1250, currency: "EUR" }));
  assert.equal(a.key(), b.key());
});

test("=== stays identity", () => {
  assert.notEqual(new Money(1, "EUR"), new Money(1, "EUR"));
  assert.equal(new Set([new Money(1, "EUR"), new Money(1, "EUR")]).size, 2);
});
