// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { PatchError, applyPatch, applyPatchFile } from "./applyAPatch.js";

// name: [the old text, the patch, the new text]
const CASES = {
  "change-a-line": ["a\nb\nc\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n a\n-b\n+B\n c\n", "a\nB\nc\n"],
  "two-hunks": ["apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n apple\n-banana\n+blueberry\n cherry\n@@ -6,2 +6,2 @@\n fig\n-grape\n+guava\n", "apple\nblueberry\ncherry\ndate\nelder\nfig\nguava\n"],
  "git-header": ["def main():\n    print(1)\n", "diff --git a/main.py b/main.py\nindex 1234567..89abcde 100644\n--- a/main.py\n+++ b/main.py\n@@ -1,2 +1,2 @@ def main():\n def main():\n-    print(1)\n+    print(2)\n", "def main():\n    print(2)\n"],
  "add-at-end": ["a\n", "@@ -1 +1,2 @@\n a\n+b\n", "a\nb\n"],
  "old-empty": ["", "--- /dev/null\n+++ b.txt\n@@ -0,0 +1,2 @@\n+a\n+b\n", "a\nb\n"],
  "delete-all": ["a\nb\n", "@@ -1,2 +0,0 @@\n-a\n-b\n", ""],
  "no-count": ["a\nb\nc\n", "@@ -2 +2 @@\n-b\n+x\n", "a\nx\nc\n"],
  "zero-context": ["apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "@@ -3 +3 @@\n-cherry\n+coconut\n@@ -6 +5,0 @@\n-fig\n@@ -7,0 +7 @@\n+honeydew\n", "apple\nbanana\ncoconut\ndate\nelder\ngrape\nhoneydew\n"],
  "insert-at-start": ["b\n", "@@ -0,0 +1 @@\n+a\n", "a\nb\n"],
  "old-no-final-newline": ["a\nb", "@@ -1,2 +1,2 @@\n a\n-b\n\\ No newline at end of file\n+b\n", "a\nb\n"],
  "new-no-final-newline": ["a\nb\n", "@@ -1,2 +1,2 @@\n a\n-b\n+b\n\\ No newline at end of file\n", "a\nb"],
  "empty-context-line": ["a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n\n-b\n+c\n", "a\n\nc\n"],
  "space-context-line": ["a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n \n-b\n+c\n", "a\n\nc\n"],
  "crlf": ["a\r\nb\r\n", "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n", "a\r\nc\r\n"],
  "header-like-line": ["x\n-- y\n", "--- old.txt\n+++ new.txt\n@@ -1,2 +1 @@\n x\n--- y\n", "x\n"],
  "non-ascii": ["caf\u00e9\n", "@@ -1 +1 @@\n-caf\u00e9\n+na\u00efve\n", "na\u00efve\n"],
};

// name: [the old text, the patch, the kind of the error]
const ERRORS = {
  "mismatch": ["a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-x\n+y\n c\n", "mismatch"],
  "malformed": ["a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-b\n", "malformed"],
};

for (const [name, [oldText, patch, want]] of Object.entries(CASES)) {
  test(name, () => {
    assert.equal(applyPatch(oldText, patch), want);
  });
}

for (const [name, [oldText, patch, kind]] of Object.entries(ERRORS)) {
  test(name, () => {
    assert.throws(() => applyPatch(oldText, patch), (e) => e instanceof PatchError && e.kind === kind);
  });
}

test("more-errors", () => {
  const kind = (k) => (e) => e instanceof PatchError && e.kind === k;
  assert.throws(() => applyPatch("a\n", "no hunk here\n"), kind("malformed"));
  assert.throws(() => applyPatch("a\n", "@@ -1 +1 @@\n*a\n"), kind("malformed"));
  assert.throws(() => applyPatch("a\n", "@@ -2 +2 @@\n-a\n+b\n"), kind("mismatch"));
});

test("many-hunks", () => {
  // 1000 lines, and 10 hunks with no context change 10 of them.
  const range = Array.from({ length: 1000 }, (_, i) => i);
  const oldText = range.map((i) => `${i}\n`).join("");
  let patch = "";
  for (let i = 50; i < 1000; i += 100) patch += `@@ -${i + 1} +${i + 1} @@\n-${i}\n+x${i}\n`;
  const want = range.map((i) => (i % 100 === 50 ? `x${i}\n` : `${i}\n`)).join("");
  assert.equal(applyPatch(oldText, patch), want);
});

const dir = mkdtempSync(join(tmpdir(), "apply-a-patch-"));
after(() => rmSync(dir, { recursive: true, force: true }));

test("apply-patch-file", () => {
  const [oldText, patch, want] = CASES["crlf"];
  writeFileSync(join(dir, "old.txt"), oldText, "utf8");
  writeFileSync(join(dir, "a.patch"), patch, "utf8");
  assert.equal(applyPatchFile(join(dir, "old.txt"), join(dir, "a.patch")), want);
});

test("not-found", () => {
  writeFileSync(join(dir, "a.patch"), "@@ -1 +1 @@\n-a\n+b\n", "utf8");
  assert.throws(() => applyPatchFile(join(dir, "missing.txt"), join(dir, "a.patch")), { code: "ENOENT" });
});
