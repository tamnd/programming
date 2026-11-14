// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { wordFrequencies } from "./wordFrequencies.ts";

// name: [the bytes of the file, n, the result]
const CASES: Record<string, [string | Buffer, number, Array<[string, number]>]> = {
  simple: ["the cat and the hat\n", 2, [["the", 2], ["and", 1]]],
  case: ["The THE the tHe\n", 5, [["the", 4]]],
  punctuation: ["it's well-dressed, isn't it?\n", 10, [["it", 2], ["dressed", 1], ["isn", 1], ["s", 1], ["t", 1], ["well", 1]]],
  empty: ["", 3, []],
  "no-letters": ["123 ... 42\n-- !!\n", 3, []],
  ties: ["b a c b a c\n", 2, [["a", 2], ["b", 2]]],
  digits: ["abc123def 42 x9\n", 10, [["abc", 1], ["def", 1], ["x", 1]]],
  "n-zero": ["the cat and the hat\n", 0, []],
  "n-above-count": ["x y x\n", 10, [["x", 2], ["y", 1]]],
  crlf: ["one two\r\ntwo\r\n", 5, [["two", 2], ["one", 1]]],
  "non-ascii": [Buffer.from("caf\u00e9 na\u00efve cafe\n", "utf8"), 10, [["caf", 1], ["cafe", 1], ["na", 1], ["ve", 1]]],
  "leading-separators": ["  ...hello, world. Hello\n", 5, [["hello", 2], ["world", 1]]],
  "no-final-newline": ["dog cat dog", 5, [["dog", 2], ["cat", 1]]],
  "not-utf8": [Buffer.from([0x6f, 0x6b, 0x20, 0xff, 0xfe, 0x20, 0x6f, 0x6b, 0x0a]), 5, [["ok", 2]]],
  "many-lines": ["a b a\n".repeat(100000), 3, [["a", 200000], ["b", 100000]]],
  // One line of 100000 words: longer than one buffer of 64 KiB.
  "long-line": ["word ".repeat(100000), 3, [["word", 100000]]],
};

const dir = mkdtempSync(join(tmpdir(), "word-frequencies-"));
after(() => rmSync(dir, { recursive: true, force: true }));

for (const [name, [data, n, expect]] of Object.entries(CASES)) {
  test(name, async () => {
    const path = join(dir, `${name}.txt`);
    writeFileSync(path, data);
    assert.deepEqual(await wordFrequencies(path, n), expect);
  });
}

test("not-found", async () => {
  await assert.rejects(wordFrequencies(join(dir, "missing.txt"), 3), { code: "ENOENT" });
});
