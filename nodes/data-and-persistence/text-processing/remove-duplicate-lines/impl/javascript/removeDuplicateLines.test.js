// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { removeDuplicateLines } from "./removeDuplicateLines.js";

const numbers = (n) => Array.from({ length: n }, (_, i) => String(i));

// name: [the text of the file, written as UTF-8, the result]
const CASES = {
  simple: ["b\na\nb\nc\na\n", ["b", "a", "c"]],
  adjacent: ["a\na\nb\n", ["a", "b"]],
  "no-duplicates": ["c\nb\na\n", ["c", "b", "a"]],
  empty: ["", []],
  "empty-lines": ["\na\n\n\n", ["", "a"]],
  "all-same": ["x\nx\nx\n", ["x"]],
  "no-final-newline": ["a\nb\na", ["a", "b"]],
  crlf: ["a\r\na\n", ["a\r", "a"]],
  case: ["A\na\nA\n", ["A", "a"]],
  whitespace: ["a \na\n a\na \n", ["a ", "a", " a"]],
  // U+00E9, then e and U+0301: the same letter, but not the same code points.
  normalization: ["\u00e9\ne\u0301\n\u00e9\n", ["\u00e9", "e\u0301"]],
  // 100000 lines with 1000 different lines.
  "many-lines": [Array.from({ length: 100000 }, (_, i) => `${i % 1000}\n`).join(""), numbers(1000)],
};

const dir = mkdtempSync(join(tmpdir(), "remove-duplicate-lines-"));
after(() => rmSync(dir, { recursive: true, force: true }));

for (const [name, [text, expect]] of Object.entries(CASES)) {
  test(name, () => {
    const path = join(dir, `${name}.txt`);
    writeFileSync(path, text, "utf8");
    assert.deepEqual(removeDuplicateLines(path), expect);
  });
}

test("not-found", () => {
  assert.throws(() => removeDuplicateLines(join(dir, "missing.txt")), { code: "ENOENT" });
});
