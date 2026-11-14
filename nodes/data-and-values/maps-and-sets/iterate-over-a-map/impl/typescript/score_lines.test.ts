// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { scoreLines } from "./score_lines.ts";

const cases: [string, Map<string, number>, string[]][] = [
  ["scores", new Map([["bob", 7], ["amy", 9], ["cal", 4]]), ["amy=9", "bob=7", "cal=4", "total=20"]],
  ["one", new Map([["x", 1]]), ["x=1", "total=1"]],
  ["empty", new Map<string, number>(), ["total=0"]],
  ["case", new Map([["b", 1], ["B", 2], ["a", 3], ["A", 4]]), ["A=4", "B=2", "a=3", "b=1", "total=10"]],
  ["negative", new Map([["a", -5], ["b", 5]]), ["a=-5", "b=5", "total=0"]],
  ["prefix", new Map([["abc", 3], ["a", 2], ["ab", 1]]), ["a=2", "ab=1", "abc=3", "total=6"]],
  ["reverse", new Map([["j", 1], ["i", 2], ["h", 3], ["g", 4], ["f", 5], ["e", 6], ["d", 7], ["c", 8], ["b", 9], ["a", 10]]), ["a=10", "b=9", "c=8", "d=7", "e=6", "f=5", "g=4", "h=3", "i=2", "j=1", "total=55"]],
];

for (const [name, scores, want] of cases) {
  test(name, () => {
    assert.deepEqual(scoreLines(scores), want);
  });
}
