// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { existsSync } from "node:fs";
import { mkdir, mkdtemp, open, readFile, rm, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { truncateFile } from "./truncateFile.js";

async function withDir(fn) {
  const dir = await mkdtemp(join(tmpdir(), "truncate-file-"));
  try {
    await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

// name: [path, length, content before, content after]
const cases = [
  ["shrink", "a.txt", 5, "alpha beta\n", "alpha"],
  ["extend", "a.txt", 8, "alpha", "alpha\0\0\0"],
  ["same-length", "a.txt", 6, "alpha\n", "alpha\n"],
  ["to-zero", "a.txt", 0, "alpha\n", ""],
  ["empty-file", "a.txt", 3, "", "\0\0\0"],
  ["name-with-spaces", "my notes.txt", 2, "alpha\n", "al"],
];

for (const [name, rel, length, before, after] of cases) {
  test(name, () =>
    withDir(async (dir) => {
      const path = join(dir, rel);
      await writeFile(path, before);
      await truncateFile(path, length);
      assert.deepEqual(await readFile(path), Buffer.from(after));
    }));
}

test("errors", () =>
  withDir(async (dir) => {
    await assert.rejects(truncateFile(join(dir, "a.txt"), 0), { code: "ENOENT" });
    assert.equal(existsSync(join(dir, "a.txt")), false);
    await mkdir(join(dir, "dir"));
    await assert.rejects(truncateFile(join(dir, "dir"), 0), { code: "EISDIR" });
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await assert.rejects(truncateFile(join(dir, "a.txt"), -1), RangeError);
    assert.equal(await readFile(join(dir, "a.txt"), "utf8"), "alpha\n");
  }));

test("open in append mode", () =>
  withDir(async (dir) => {
    const path = join(dir, "app.log");
    await writeFile(path, "alpha beta\n");
    const log = await open(path, "a");
    try {
      await truncateFile(path, 0);
      await log.write("new\n");
    } finally {
      await log.close();
    }
    assert.equal(await readFile(path, "utf8"), "new\n");
  }));

test("open after the new end", () =>
  withDir(async (dir) => {
    const path = join(dir, "data.bin");
    await writeFile(path, "alpha beta\n");
    const f = await open(path, "r+");
    try {
      await truncateFile(path, 5);
      await f.write("X", 11);
    } finally {
      await f.close();
    }
    assert.deepEqual(await readFile(path), Buffer.from("alpha\0\0\0\0\0\0X"));
  }));
