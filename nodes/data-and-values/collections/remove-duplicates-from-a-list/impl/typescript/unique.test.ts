// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { type User, unique, uniqueByEmail } from "./unique.ts";

const cases: Record<string, [string[], string[]]> = {
  repeats: [["apple", "pear", "apple", "fig", "pear"], ["apple", "pear", "fig"]],
  words: [["to", "be", "or", "not", "to", "be"], ["to", "be", "or", "not"]],
  empty: [[], []],
  single: [["a"], ["a"]],
  "all-same": [["x", "x", "x"], ["x"]],
  "no-duplicates": [["c", "a", "b"], ["c", "a", "b"]],
  "not-adjacent": [["a", "b", "a", "b"], ["a", "b"]],
  adjacent: [["a", "a", "b", "b", "a"], ["a", "b"]],
  "empty-string": [["", "a", "", "b"], ["", "a", "b"]],
  case: [["a", "A", "a"], ["a", "A"]],
  normalization: [["\u00e9", "e\u0301", "\u00e9"], ["\u00e9", "e\u0301"]],
  whitespace: [[" a", "a", "a ", " a"], [" a", "a", "a "]],
  "number-like": [["1", "01", "1.0", "1"], ["1", "01", "1.0"]],
  "non-ascii": [["日本", "🙂", "日本", "🙂", "ü"], ["日本", "🙂", "ü"]],
};

for (const [name, [given, want]] of Object.entries(cases)) {
  test(name, () => {
    const input = [...given];
    assert.deepEqual(unique(input), want);
    assert.deepEqual(input, given, "the input must not change");
  });
}

type Row = [string, string];

/** Makes users from [name, email] rows. */
const users = (rows: readonly Row[]): User[] => rows.map(([name, email]) => ({ name, email }));

// The cases of cases/by-key.jsonl.
const byKey: Record<string, [Row[], Row[]]> = {
  "case-insensitive": [
    [["Ada", "ada@example.com"], ["Bob", "bob@example.com"], ["Ada L.", "ADA@Example.com"]],
    [["Ada", "ada@example.com"], ["Bob", "bob@example.com"]],
  ],
  empty: [[], []],
  "same-name-other-email": [
    [["Sam", "sam@a.example"], ["Sam", "sam@b.example"]],
    [["Sam", "sam@a.example"], ["Sam", "sam@b.example"]],
  ],
  "first-record-wins": [[["Old", "x@example.com"], ["New", "X@EXAMPLE.COM"]], [["Old", "x@example.com"]]],
  "dots-differ": [
    [["Al", "a.b@example.com"], ["Bo", "ab@example.com"]],
    [["Al", "a.b@example.com"], ["Bo", "ab@example.com"]],
  ],
  "not-adjacent": [
    [["A", "a@example.com"], ["B", "b@example.com"], ["C", "A@example.com"], ["D", "B@EXAMPLE.com"]],
    [["A", "a@example.com"], ["B", "b@example.com"]],
  ],
};

for (const [name, [given, want]] of Object.entries(byKey)) {
  test(`by-key ${name}`, () => {
    assert.deepEqual(uniqueByEmail(users(given)), users(want));
  });
}
