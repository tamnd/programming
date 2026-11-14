// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { sortLines } from "./sortLines.ts";

// name: [the text of the file, written as UTF-8, the result]
const CASES: Record<string, [string, string[]]> = {
  simple: ["pear\napple\nfig\n", ["apple", "fig", "pear"]],
  case: ["b\nB\na\nA\n", ["A", "B", "a", "b"]],
  numbers: ["10\n9\n100\n", ["10", "100", "9"]],
  empty: ["", []],
  "one-empty-line": ["\n", [""]],
  "empty-lines": ["b\n\na\n\n", ["", "", "a", "b"]],
  duplicates: ["b\na\nb\n", ["a", "b", "b"]],
  "no-final-newline": ["b\na", ["a", "b"]],
  crlf: ["b\r\na\r\n", ["a\r", "b\r"]],
  prefix: ["ab\na\n", ["a", "ab"]],
  whitespace: ["b\n a\n\tc\n", ["\tc", " a", "b"]],
  "non-ascii": ["\u00e9\nz\ne\n", ["e", "z", "\u00e9"]],
  // U+FF61 is below U+1F600, but its UTF-16 code unit is above 0xD83D.
  supplementary: ["\u{1f600}\n\uff61\n", ["\uff61", "\u{1f600}"]],
  "other-line-breaks": ["b\u2028a\nb\x1ca\nb\x0ca\n", ["b\x0ca", "b\x1ca", "b\u2028a"]],
  "many-lines": ["b\na\n".repeat(50000), [...Array(50000).fill("a"), ...Array(50000).fill("b")]],
};

const dir = mkdtempSync(join(tmpdir(), "sort-lines-"));
after(() => rmSync(dir, { recursive: true, force: true }));

for (const [name, [text, expect]] of Object.entries(CASES)) {
  test(name, () => {
    const path = join(dir, `${name}.txt`);
    writeFileSync(path, text, "utf8");
    assert.deepEqual(sortLines(path), expect);
  });
}

test("not-found", () => {
  assert.throws(() => sortLines(join(dir, "missing.txt")), { code: "ENOENT" });
});
