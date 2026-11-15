// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { diffFiles, diffLines } from "./diffTwoTexts.ts";

// name: [the old text, the new text, the edit script]
const CASES: Record<string, [string, string, string]> = {
  "simple": ["a\nb\nc\n", "a\nc\nd\n", " a\n-b\n c\n+d\n"],
  "change-a-line": ["one\ntwo\nthree\n", "one\n2\nthree\n", " one\n-two\n+2\n three\n"],
  "identical": ["a\nb\n", "a\nb\n", " a\n b\n"],
  "lines-of-a-poem": ["The cat sat\non the mat.\nIt was flat.\n", "The cat sat\non the hat.\nIt was flat.\nThe end.\n", " The cat sat\n-on the mat.\n+on the hat.\n It was flat.\n+The end.\n"],
  "both-empty": ["", "", ""],
  "old-empty": ["", "a\nb\n", "+a\n+b\n"],
  "new-empty": ["a\nb\n", "", "-a\n-b\n"],
  "all-different": ["a\nb\n", "c\nd\n", "-a\n-b\n+c\n+d\n"],
  "insert-at-start": ["b\nc\n", "a\nb\nc\n", "+a\n b\n c\n"],
  "delete-at-end": ["a\nb\nc\n", "a\nb\n", " a\n b\n-c\n"],
  "swap": ["a\nb\n", "b\na\n", "-a\n b\n+a\n"],
  "repeated-lines": ["a\na\nb\n", "a\nb\nb\n", " a\n-a\n b\n+b\n"],
  "empty-line": ["a\n\nb\n", "a\nb\n", " a\n-\n b\n"],
  "no-final-newline": ["a\nb", "a\nb\n", " a\n b\n"],
  "crlf": ["a\r\nb\n", "a\nb\n", "-a\r\n+a\n b\n"],
  "trailing-space": ["a \nb\n", "a\nb\n", "-a \n+a\n b\n"],
  "normalization": ["caf\u00e9\n", "cafe\u0301\n", "-caf\u00e9\n+cafe\u0301\n"],
};

for (const [name, [oldText, newText, want]] of Object.entries(CASES)) {
  test(name, () => {
    assert.equal(diffLines(oldText, newText), want);
  });
}

test("many-lines", () => {
  // 1000 lines, and 10 of them change.
  const range = Array.from({ length: 1000 }, (_, i) => i);
  const oldText = range.map((i) => `${i}\n`).join("");
  const newText = range.map((i) => (i % 100 === 50 ? `x${i}\n` : `${i}\n`)).join("");
  const lines = diffLines(oldText, newText).split("\n");
  assert.equal(lines.length, 1011);
  assert.deepEqual(lines.slice(49, 52), [" 49", "-50", "+x50"]);
  assert.equal(lines[1009], " 999");
});

const dir = mkdtempSync(join(tmpdir(), "diff-two-texts-"));
after(() => rmSync(dir, { recursive: true, force: true }));

test("diff-files", () => {
  const [oldText, newText, want] = CASES["crlf"];
  writeFileSync(join(dir, "old.txt"), oldText, "utf8");
  writeFileSync(join(dir, "new.txt"), newText, "utf8");
  assert.equal(diffFiles(join(dir, "old.txt"), join(dir, "new.txt")), want);
});

test("not-found", () => {
  writeFileSync(join(dir, "new.txt"), "a\n", "utf8");
  assert.throws(() => diffFiles(join(dir, "missing.txt"), join(dir, "new.txt")), { code: "ENOENT" });
});
