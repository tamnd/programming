// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { areAnagrams } from "./anagrams.ts";

const cases: [string, string, string, boolean][] = [
  ["listen-silent", "listen", "silent", true],
  ["dormitory", "Dormitory", "dirty room", true],
  ["hello-world", "hello", "world", false],
  ["both-empty", "", "", true],
  ["one-empty", "a", "", false],
  ["same-set-other-counts", "aab", "abb", false],
  ["extra-letter", "abc", "abcd", false],
  ["same-length-other-letters", "abc", "abd", false],
  ["case", "Listen", "SILENT", true],
  ["same-word", "stop", "stop", true],
  ["gentleman", "a gentleman", "elegant man", true],
  ["they-see", "The eyes!", "They see.", true],
  ["punctuation-only", "!?", "...", true],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    assert.equal(areAnagrams(a, b), want);
    assert.equal(areAnagrams(b, a), want);
  });
}
