// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { appendFile, mkdir, mkdtemp, rm, symlink, utimes, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { fileMtime } from "./fileMtime.ts";

async function withDir(fn: (dir: string) => Promise<void>): Promise<void> {
  const dir = await mkdtemp(join(tmpdir(), "file-mtime-"));
  try {
    await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

// name, path, mtime in milliseconds, result. utimes takes a Date: it uses the
// present time for a negative number of seconds.
const cases: [string, string, number, number][] = [
  ["whole-second", "a.txt", 1709296496000, 1709296496],
  ["sub-second", "a.txt", 1709296496500, 1709296496],
  ["epoch", "a.txt", 0, 0],
  ["before-1970", "a.txt", -86400000, -86400],
  ["before-1970-sub-second", "a.txt", -1500, -2],
  ["after-2038", "a.txt", 4102444800000, 4102444800],
  ["directory", "dir", 1709296496000, 1709296496],
  ["name-with-spaces", "my notes.txt", 1709296496000, 1709296496],
];

for (const [name, rel, mtime, want] of cases) {
  test(name, () =>
    withDir(async (dir) => {
      const path = join(dir, rel);
      if (rel === "dir") await mkdir(path);
      else await writeFile(path, "alpha\n");
      await utimes(path, new Date(mtime), new Date(mtime));
      assert.equal(await fileMtime(path), want);
    }),
  );
}

test("errors", () =>
  withDir(async (dir) => {
    await assert.rejects(fileMtime(join(dir, "a.txt")), { code: "ENOENT" });
  }));

test("symbolic link", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await utimes(join(dir, "a.txt"), 0, 1709296496);
    await symlink("a.txt", join(dir, "link"));
    assert.equal(await fileMtime(join(dir, "link")), 1709296496);
  }));

test("write sets the present time", () =>
  withDir(async (dir) => {
    const path = join(dir, "a.txt");
    await writeFile(path, "alpha\n");
    await utimes(path, 0, 0);
    const before = Math.floor(Date.now() / 1000) - 1;
    await appendFile(path, "beta\n");
    assert.ok((await fileMtime(path)) >= before);
  }));
