// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { balanced } from "./balanced.ts";

const deep = "(".repeat(1000) + ")".repeat(1000);

const cases: [string, string, boolean][] = [
  ["pair", "()", true],
  ["nested", "{[()()]}", true],
  ["code", "f(a[0], {b: 1})", true],
  ["crossed", "([)]", false],
  ["empty", "", true],
  ["no-brackets", "abc", true],
  ["unclosed", "((", false],
  ["extra-close", "())", false],
  ["close-first", ")(", false],
  ["wrong-kind", "(]", false],
  ["deep", deep, true],
];

for (const [name, text, want] of cases) {
  test(name, () => {
    assert.equal(balanced(text), want);
  });
}
