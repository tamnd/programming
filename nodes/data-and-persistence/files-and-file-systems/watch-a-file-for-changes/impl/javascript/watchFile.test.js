// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.
import assert from "node:assert/strict";
import * as fs from "node:fs";
import * as os from "node:os";
import * as path from "node:path";
import { test } from "node:test";

import { FileWatcher, watch, watchChanges } from "./watchFile.js";

const CASES = {
  "create": [["create:hello\n"], ["created"]],
  "create-then-write": [["create:a\n", "write:a much longer line\n"], ["created", "modified"]],
  "create-then-delete": [["create:a\n", "delete"], ["created", "deleted"]],
  "append": [["create:a\n", "append:b\n", "append:c\n"], ["created", "modified", "modified"]],
  "no-change": [["create:a\n", "none", "none"], ["created", "none", "none"]],
  "same-size": [["create:abc\n", "write:xyz\n"], ["created", "modified"]],
  "touch-only": [["create:a\n", "touch"], ["created", "modified"]],
  "replace-by-rename": [["create:old\n", "replace:new\n"], ["created", "modified"]],
  "delete-and-create": [["create:a\n", "recreate:b\n"], ["created", "modified"]],
  "never-created": [["none"], ["none"]],
};

function tempDir() {
  return fs.mkdtempSync(path.join(os.tmpdir(), "watch-"));
}

for (const [name, [steps, expect]] of Object.entries(CASES)) {
  test(name, () => {
    const dir = tempDir();
    try {
      assert.deepEqual(watchChanges(path.join(dir, "watched.txt"), steps), expect);
    } finally {
      fs.rmSync(dir, { recursive: true, force: true });
    }
  });
}

test("a directory is an error", () => {
  const dir = tempDir();
  try {
    assert.throws(() => watchChanges(path.join(dir, "watched.txt"), ["mkdir"]), { code: "EISDIR" });
  } finally {
    fs.rmSync(dir, { recursive: true, force: true });
  }
});

test("the same size and the same mtime hide a change", () => {
  const dir = tempDir();
  try {
    const p = path.join(dir, "watched.txt");
    fs.writeFileSync(p, "abc\n");
    fs.utimesSync(p, 5, 5);
    const watcher = new FileWatcher(p);
    fs.writeFileSync(p, "xyz\n");
    fs.utimesSync(p, 5, 5);
    assert.equal(watcher.poll(), "none");
    fs.writeFileSync(p, "xyz!\n");
    assert.equal(watcher.poll(), "modified");
  } finally {
    fs.rmSync(dir, { recursive: true, force: true });
  }
});

test("watch calls once for a burst of changes", async () => {
  const dir = tempDir();
  const p = path.join(dir, "watched.txt");
  const calls = [];
  const stop = watch(p, (f) => calls.push(f), { interval: 10, quiet: 500 });
  try {
    for (let i = 0; i < 5; i++) {
      fs.writeFileSync(p, "x".repeat(i + 1));
      await new Promise((r) => setTimeout(r, 20));
    }
    const deadline = Date.now() + 5000;
    while (calls.length === 0 && Date.now() < deadline) {
      await new Promise((r) => setTimeout(r, 10));
    }
    await new Promise((r) => setTimeout(r, 600));
    assert.deepEqual(calls, [p]);
  } finally {
    stop();
    fs.rmSync(dir, { recursive: true, force: true });
  }
});
