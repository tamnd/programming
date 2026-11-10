// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

import assert from "node:assert/strict";
import { test } from "node:test";
import type { WordSource } from "./randomNumbers.ts";
import { intInRange, secureWords, seededWords, shuffle, tokenHex } from "./randomNumbers.ts";

const rangeCases: [string, number[], bigint, bigint, number, bigint[]][] = [
  ["coin", [5, 2, 7], 0n, 1n, 3, [1n, 0n, 1n]],
  ["no-results", [], 1n, 6n, 0, []],
  ["die", [7, 12, 1000000, 4294967295], 1n, 6n, 4, [2n, 1n, 5n, 4n]],
  ["negative-range", [100, 2000000000, 4294967294], -10n, 10n, 3, [6n, -5n, -8n]],
  ["one-value", [0, 99], 5n, 5n, 2, [5n, 5n]],
  ["reject-low-words", [0, 3, 4], 1n, 6n, 1, [5n]],
  ["accept-least", [3, 4, 9], 0n, 5n, 2, [4n, 3n]],
  ["power-of-two", [0, 4294967295], 0n, 7n, 2, [0n, 7n]],
  ["worst-case", [2147483646, 2147483647], 0n, 2147483648n, 1, [2147483647n]],
  ["full-unsigned-32", [0, 4294967295, 123456789], 0n, 4294967295n, 3, [0n, 4294967295n, 123456789n]],
  ["full-signed-32", [0, 2147483648, 4294967295], -2147483648n, 2147483647n, 3, [-2147483648n, 0n, 2147483647n]],
  ["near-int64-max", [4294967295, 6], 9223372036854775798n, 9223372036854775807n, 2, [9223372036854775803n, 9223372036854775804n]],
  ["near-int64-min", [4294967295, 9], -9223372036854775808n, -9223372036854775799n, 2, [-9223372036854775803n, -9223372036854775799n]],
  ["wide-int64", [4294967295, 0], 9223372032559808512n, 9223372036854775807n, 2, [9223372036854775807n, 9223372032559808512n]],
];

const shuffleCases: [string, number[], string[], string[]][] = [
  ["empty", [], [], []],
  ["one-item", [], ["a"], ["a"]],
  ["two-items-swap", [0], ["a", "b"], ["b", "a"]],
  ["two-items-keep", [1], ["a", "b"], ["a", "b"]],
  ["five-items", [7, 3, 4, 1], ["a", "b", "c", "d", "e"], ["a", "e", "b", "d", "c"]],
  ["reject-in-shuffle", [0, 2, 0], ["a", "b", "c"], ["b", "a", "c"]],
  ["duplicates", [1, 1], ["x", "x", "y"], ["x", "y", "x"]],
  ["non-ascii", [4294967295, 4294967294, 0], ["é", "日本", "🙂", "a"], ["日本", "é", "🙂", "a"]],
];

/** Returns a word source that gives words in order, and a check that all were used. */
function fakeWords(words: number[]): [WordSource, () => boolean] {
  let next = 0;
  const source = () => {
    assert.ok(next < words.length, "no more words");
    return words[next++] as number;
  };
  return [source, () => next === words.length];
}

for (const [name, words, lo, hi, count, want] of rangeCases) {
  test(`intInRange ${name}`, () => {
    const [source, usedAll] = fakeWords(words);
    const got = Array.from({ length: count }, () => intInRange(source, lo, hi));
    assert.deepEqual(got, want);
    assert.ok(usedAll());
  });
}

for (const [name, words, items, want] of shuffleCases) {
  test(`shuffle ${name}`, () => {
    const [source, usedAll] = fakeWords(words);
    shuffle(source, items);
    assert.deepEqual(items, want);
    assert.ok(usedAll());
  });
}

test("a bad range throws", () => {
  assert.throws(() => intInRange(() => 0, 1n, 0n), RangeError);
  assert.throws(() => intInRange(() => 0, 0n, 1n << 32n), RangeError);
});

test("SplitMix64 gives the words of the reference code", () => {
  const source = seededWords(42n);
  const got = Array.from({ length: 5 }, source);
  assert.deepEqual(got, [3184996902, 686809907, 1196582743, 1478287871, 163338330]);
});

test("the same seed gives the same words", () => {
  const a = seededWords(7n);
  const b = seededWords(7n);
  const c = seededWords(8n);
  const first = Array.from({ length: 10 }, a);
  assert.deepEqual(Array.from({ length: 10 }, b), first);
  assert.notDeepEqual(Array.from({ length: 10 }, c), first);
});

test("secure words give values in range", () => {
  const source = secureWords();
  for (let i = 0; i < 100; i++) {
    const v = intInRange(source, 1n, 6n);
    assert.ok(v >= 1n && v <= 6n);
  }
});

for (const nbytes of [0, 1, 16, 32]) {
  test(`tokenHex ${nbytes}`, () => {
    assert.match(tokenHex(nbytes), new RegExp(`^[0-9a-f]{${2 * nbytes}}$`));
  });
}

test("tokens differ", () => {
  assert.notEqual(tokenHex(16), tokenHex(16));
});
