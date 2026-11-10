// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

import assert from "node:assert/strict";
import { mkdirSync, mkdtempSync, rmSync, symlinkSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { listEntries } from "./listEntries.ts";

const TREE = ["docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md"];

const CASES: Record<string, [string[], boolean, string[]]> = {
  flat: [["b.txt", "a.txt", "c.txt"], false, ["a.txt", "b.txt", "c.txt"]],
  nested: [TREE, true, TREE],
  "nested-shallow": [TREE, false, ["docs/", "readme.md"]],
  empty: [[], true, []],
  "empty-subdir": [["empty/"], true, ["empty/"]],
  hidden: [[".env", ".git/", ".git/config", "main.go"], true, [".env", ".git/", ".git/config", "main.go"]],
  "case-order": [["a.txt", "B.txt", "c.txt"], false, ["B.txt", "a.txt", "c.txt"]],
  "dot-before-slash": [["a/", "a/x", "a.txt", "a-b"], true, ["a-b", "a.txt", "a/", "a/x"]],
  "symlink-to-dir": [["real/", "real/f", "link -> real"], true, ["link", "real/", "real/f"]],
  "symlink-to-file": [["f", "g -> f"], true, ["f", "g"]],
  "broken-symlink": [["dangling -> nowhere"], true, ["dangling"]],
  "symlink-loop": [["d/", "d/up -> .."], true, ["d/", "d/up"]],
  spaces: [["my file.txt", "two  spaces/", "two  spaces/x"], true, ["my file.txt", "two  spaces/", "two  spaces/x"]],
  "non-ascii": [["été.txt", "zebra.txt", "apple.txt"], false, ["apple.txt", "zebra.txt", "été.txt"]],
};

const base = mkdtempSync(join(tmpdir(), "list-entries-"));
after(() => rmSync(base, { recursive: true, force: true }));

let count = 0;
function makeTree(tree: string[]): string {
  const root = join(base, String(count++));
  mkdirSync(root);
  for (const entry of tree) {
    if (entry.includes(" -> ")) {
      const [name, target] = entry.split(" -> ") as [string, string];
      symlinkSync(target, join(root, name));
    } else if (entry.endsWith("/")) {
      mkdirSync(join(root, entry));
    } else {
      writeFileSync(join(root, entry), "");
    }
  }
  return root;
}

for (const [name, [tree, recursive, want]] of Object.entries(CASES)) {
  test(name, async () => {
    assert.deepEqual(await listEntries(makeTree(tree), recursive), want);
  });
}

test("missing", async () => {
  await assert.rejects(listEntries(join(makeTree([]), "missing"), true), { code: "ENOENT" });
});

test("not-a-directory", async () => {
  await assert.rejects(listEntries(join(makeTree(["notes.txt"]), "notes.txt"), true), { code: "ENOTDIR" });
});
