// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { caesar } from "./caesar.ts";

const cases: [string, string, number, string][] = [
  ["hi-2", "HI", 2, "JK"],
  ["hi-20", "HI", 20, "BC"],
  ["rot13", "Hello, World!", 13, "Uryyb, Jbeyq!"],
  ["empty", "", 3, ""],
  ["zero", "abc", 0, "abc"],
  ["full-turn", "abc", 26, "abc"],
  ["minus-one", "abc", -1, "zab"],
  ["decode", "Uryyb, Jbeyq!", -13, "Hello, World!"],
  ["above-26", "xyz", 27, "yza"],
  ["wrap", "Zz", 1, "Aa"],
  ["non-letters", "123 !? [`{@", 5, "123 !? [`{@"],
  ["non-ascii", "café ü", 1, "dbgé ü"],
  ["large", "abc", 9007199254740991, "fgh"],
  ["large-negative", "abc", -9007199254740991, "vwx"],
];

for (const [name, text, shift, want] of cases) {
  test(name, () => {
    assert.equal(caesar(text, shift), want);
    assert.equal(caesar(want, -shift), text);
  });
}
