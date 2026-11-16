// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { wordCount } from "./words.js";

const cases = [
  ["three-words", "the quick fox", 3],
  ["one-word", "hello", 1],
  ["punctuation", "Hello, world! How are you?", 5],
  ["empty", "", 0],
  ["blank", " \t\n ", 0],
  ["mixed-whitespace", "  two\twords\r\nand three  ", 4],
  ["non-ascii", "naïve café 東京", 3],
];

for (const [name, text, want] of cases) {
  test(name, () => {
    assert.equal(wordCount(text), want);
  });
}

test("every string has the method", () => {
  assert.equal("one two".wordCount(), 2);
  assert.equal(String("a b c").wordCount(), 3);
});

test("the method is not enumerable", () => {
  const keys = [];
  for (const k in "ab") {
    keys.push(k);
  }
  assert.deepEqual(keys, ["0", "1"]);
  assert.equal(Object.keys(String.prototype).includes("wordCount"), false);
});
