// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { existsSync, lstatSync } from "node:fs";
import { chmod, mkdir, mkdtemp, open, readFile, rm, symlink, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { deleteFile } from "./deleteFile.ts";

async function withDir(fn: (dir: string) => Promise<void>): Promise<void> {
  const dir = await mkdtemp(join(tmpdir(), "delete-file-"));
  try {
    await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

function lexists(path: string): boolean {
  try {
    lstatSync(path);
    return true;
  } catch {
    return false;
  }
}

// name: [path, content before the call or null, result]
const cases: Record<string, [string, string | null, boolean]> = {
  delete: ["a.txt", "alpha\n", true],
  missing: ["a.txt", null, false],
  "empty-file": ["a.txt", "", true],
  "missing-parent": ["no-such-dir/a.txt", null, false],
  "name-with-spaces": ["my notes.txt", "alpha\n", true],
  "non-ascii-name": ["café 日本.txt", "alpha\n", true],
};

for (const [name, [rel, content, want]] of Object.entries(cases)) {
  test(name, () =>
    withDir(async (dir) => {
      const path = join(dir, rel);
      if (content !== null) await writeFile(path, content);
      assert.equal(await deleteFile(path), want);
      assert.equal(lexists(path), false);
    }));
}

test("through a file", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "a.txt"), "alpha\n");
    assert.equal(await deleteFile(join(dir, "a.txt", "b.txt")), false);
    assert.equal(existsSync(join(dir, "a.txt")), true);
  }));

test("directory", () =>
  withDir(async (dir) => {
    await mkdir(join(dir, "dir"));
    await assert.rejects(deleteFile(join(dir, "dir")));
    assert.equal(lstatSync(join(dir, "dir")).isDirectory(), true);
  }));

test("links", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await mkdir(join(dir, "dir"));
    await symlink("a.txt", join(dir, "to-file"));
    await symlink("dir", join(dir, "to-dir"));
    await symlink("missing.txt", join(dir, "broken"));
    for (const link of ["to-file", "to-dir", "broken"]) {
      assert.equal(await deleteFile(join(dir, link)), true);
      assert.equal(lexists(join(dir, link)), false);
    }
    assert.equal(await readFile(join(dir, "a.txt"), "utf8"), "alpha\n");
    assert.equal(lstatSync(join(dir, "dir")).isDirectory(), true);
  }));

test("read-only file", () =>
  withDir(async (dir) => {
    const path = join(dir, "a.txt");
    await writeFile(path, "alpha\n");
    await chmod(path, 0o444);
    assert.equal(await deleteFile(path), true);
    assert.equal(lexists(path), false);
  }));

test("directory with no write permission", { skip: process.getuid?.() === 0 }, () =>
  withDir(async (dir) => {
    const locked = join(dir, "locked");
    await mkdir(locked);
    const path = join(locked, "a.txt");
    await writeFile(path, "alpha\n");
    await chmod(locked, 0o500);
    try {
      await assert.rejects(deleteFile(path), { code: "EACCES" });
      assert.equal(existsSync(path), true);
    } finally {
      await chmod(locked, 0o700);
    }
  }));

test("open file", () =>
  withDir(async (dir) => {
    const path = join(dir, "a.txt");
    await writeFile(path, "alpha\n");
    const f = await open(path);
    try {
      assert.equal(await deleteFile(path), true);
      assert.equal(lexists(path), false);
      assert.equal(await f.readFile("utf8"), "alpha\n"); // the data stays until the close
    } finally {
      await f.close();
    }
  }));
