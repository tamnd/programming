// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { split, splitRegex, splitWhitespace } from "./split.ts";

const cases: [string, string, string, number, string[]][] = [
  ["comma", "a,b,c", ",", 0, ["a", "b", "c"]],
  ["csv-line", "Ada,1815,London", ",", 0, ["Ada", "1815", "London"]],
  ["multi-char-separator", "a::b::c", "::", 0, ["a", "b", "c"]],
  ["space-separator", "one two  three", " ", 0, ["one", "two", "", "three"]],
  ["limit-two", "key=value=more", "=", 2, ["key", "value=more"]],
  ["empty", "", ",", 0, [""]],
  ["no-separator", "abc", ",", 0, ["abc"]],
  ["leading-separator", ",a,b", ",", 0, ["", "a", "b"]],
  ["trailing-separators", "a,b,,", ",", 0, ["a", "b", "", ""]],
  ["adjacent-separators", "a,,b", ",", 0, ["a", "", "b"]],
  ["only-separator", ",", ",", 0, ["", ""]],
  ["overlapping-match", "aaa", "aa", 0, ["", "a"]],
  ["dot-separator", "192.168.0.1", ".", 0, ["192", "168", "0", "1"]],
  ["pipe-separator", "a|b|c", "|", 0, ["a", "b", "c"]],
  ["non-ascii", "día→naïve→日本→🙂", "→", 0, ["día", "naïve", "日本", "🙂"]],
  ["limit-one", "a,b,c", ",", 1, ["a,b,c"]],
  ["limit-above-count", "a,b", ",", 5, ["a", "b"]],
  ["limit-keeps-trailing-empty", "a,b,", ",", 3, ["a", "b", ""]],
];

for (const [name, s, sep, limit, want] of cases) {
  test(name, () => assert.deepEqual(split(s, sep, limit), want));
}

const whitespaceCases: [string, string, string[]][] = [
  ["single-spaces", "a b c", ["a", "b", "c"]],
  ["runs-and-ends", "  lead  and trail  ", ["lead", "and", "trail"]],
  ["tabs-and-newlines", "a\t\tb\nc\n", ["a", "b", "c"]],
  ["one-word", "word", ["word"]],
  ["empty", "", []],
  ["only-white-space", " \t\n ", []],
];

for (const [name, s, want] of whitespaceCases) {
  test(`whitespace ${name}`, () => assert.deepEqual(splitWhitespace(s), want));
}

const regexCases: [string, string, string, string[]][] = [
  ["comma-or-semicolon", "a, b;c ,d", "[ ]*[,;][ ]*", ["a", "b", "c", "d"]],
  ["digit-runs", "a1b22c333", "[0-9]+", ["a", "b", "c", ""]],
  ["adjacent-matches", "a,;b", "[,;]", ["a", "", "b"]],
  ["leading-and-trailing", ";a;", ";", ["", "a", ""]],
  ["empty", "", "[,;]", [""]],
  ["no-match", "abc", "[0-9]+", ["abc"]],
];

for (const [name, s, pattern, want] of regexCases) {
  test(`regex ${name}`, () => assert.deepEqual(splitRegex(s, pattern), want));
}
