// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { wordCount } from "./wordCount.ts";

const CHUNK = 64 * 1024; // the default highWaterMark of a file stream

// name: [the text of the file, lines, words, bytes]
const CASES = {
  simple: ["one two three\n", 1, 3, 14],
  "multiple-spaces": ["  a   b  \n", 1, 2, 10],
  "tabs-and-lines": ["a\tb\nc\n", 2, 3, 6],
  empty: ["", 0, 0, 0],
  "only-white-space": [" \n\t\n", 2, 0, 4],
  "no-final-newline": ["one two", 0, 2, 7],
  crlf: ["a b\r\nc\r\n", 2, 3, 8],
  "non-ascii": ["na\u00efve caf\u00e9\n", 1, 2, 13],
  "unicode-spaces": ["a\u00a0b\u3000c\u2028d\n", 1, 4, 13],
  "not-white-space": ["a\u200bb\u00adc\ufeffd\n", 1, 1, 13],
  "control-separators": ["a\x1cb\x1fc\n", 1, 1, 6],
  nel: ["a\u0085b\n", 1, 2, 5],
  // 100000 lines: more than one chunk of 64 KiB.
  "many-lines": ["a b\n".repeat(100000), 100000, 200000, 400000],
  // The chunk ends inside the 3 bytes of U+3000.
  "chunk-boundary": ["x".repeat(CHUNK - 1) + "\u3000y\n", 1, 2, CHUNK + 4],
};

// name: the bytes of a file that is not valid UTF-8
const INVALID = {
  "invalid-bytes": [0x6f, 0x6b, 0xff, 0xfe, 0x0a],
  truncated: [0x6f, 0x6b, 0x20, 0xe2, 0x82],
  surrogate: [0x61, 0xed, 0xa0, 0x80, 0x62, 0x0a],
};

const dir = mkdtempSync(join(tmpdir(), "word-count-"));
after(() => rmSync(dir, { recursive: true, force: true }));

for (const [name, [text, lines, words, bytes]] of Object.entries(CASES)) {
  test(name, async () => {
    const path = join(dir, `${name}.txt`);
    writeFileSync(path, text, "utf8");
    assert.deepEqual(await wordCount(path), { lines, words, bytes });
  });
}

for (const [name, data] of Object.entries(INVALID)) {
  test(name, async () => {
    const path = join(dir, `${name}.txt`);
    writeFileSync(path, Buffer.from(data));
    await assert.rejects(wordCount(path), TypeError);
  });
}

test("missing file", async () => {
  await assert.rejects(wordCount(join(dir, "missing.txt")), { code: "ENOENT" });
});
