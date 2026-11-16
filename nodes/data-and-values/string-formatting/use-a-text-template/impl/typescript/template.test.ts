// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { substitute } from "./template.ts";

const cases: [string, string, string[], string[], string][] = [
  ["greeting", "Hello, $name!", ["name"], ["Ada"], "Hello, Ada!"],
  ["braces", "${item}s cost $$${price}", ["item", "price"], ["Apple", "3"], "Apples cost $3"],
  ["two-uses", "$a and $a again", ["a"], ["x"], "x and x again"],
  ["no-placeholders", "plain text", [], [], "plain text"],
  ["empty-template", "", ["x"], ["1"], ""],
  ["dollar-escape", "$$name", ["name"], ["Ada"], "$name"],
  ["adjacent-text", "${n}th", ["n"], ["4"], "4th"],
  ["value-with-dollar", "$v", ["v"], ["$x and ${y}"], "$x and ${y}"],
  ["empty-value", "[$v]", ["v"], [""], "[]"],
  ["underscore-name", "$first_name $last_name", ["first_name", "last_name"], ["Grace", "Hopper"], "Grace Hopper"],
  ["name-ends-at-punct", "$a.$b", ["a", "b"], ["1", "2"], "1.2"],
  ["unused-value", "$a", ["a", "b"], ["1", "2"], "1"],
];

for (const [name, template, names, values, want] of cases) {
  test(name, () => {
    assert.equal(substitute(template, names, values), want);
  });
}

test("unknown-name", () => {
  assert.throws(() => substitute("Hi $who", ["name"], ["Ada"]), /unknown name: who/);
});
