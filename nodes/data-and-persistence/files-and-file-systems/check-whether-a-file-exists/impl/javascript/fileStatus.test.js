// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { execFileSync } from "node:child_process";
import { existsSync } from "node:fs";
import { chmod, mkdir, mkdtemp, rm, symlink, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { fileStatus } from "./fileStatus.js";

async function withTree(fn) {
  const dir = await mkdtemp(join(tmpdir(), "file-status-"));
  try {
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await writeFile(join(dir, "empty.txt"), "");
    await mkdir(join(dir, "dir"));
    await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

const cases = {
  "a.txt": "file",
  "empty.txt": "file",
  dir: "directory",
  "dir/": "directory",
  "missing.txt": "missing",
  "no-such-dir/a.txt": "missing",
  "a.txt/b.txt": "missing",
};

for (const [name, want] of Object.entries(cases)) {
  // join keeps a trailing slash.
  test(name, () => withTree(async (dir) => assert.equal(await fileStatus(join(dir, name)), want)));
}

test("symbolic links", () =>
  withTree(async (dir) => {
    await symlink("a.txt", join(dir, "to-file"));
    await symlink("dir", join(dir, "to-dir"));
    await symlink("missing.txt", join(dir, "to-nothing"));
    assert.equal(await fileStatus(join(dir, "to-file")), "file");
    assert.equal(await fileStatus(join(dir, "to-dir")), "directory");
    assert.equal(await fileStatus(join(dir, "to-nothing")), "broken-link");
  }));

test("device", { skip: !existsSync("/dev/null") && "no /dev/null" }, async () => {
  assert.equal(await fileStatus("/dev/null"), "other");
});

test("named pipe", () =>
  withTree(async (dir) => {
    execFileSync("mkfifo", [join(dir, "pipe")]);
    assert.equal(await fileStatus(join(dir, "pipe")), "other");
  }));

test("permission error", { skip: process.getuid?.() === 0 && "root can search each directory" }, () =>
  withTree(async (dir) => {
    const locked = join(dir, "locked");
    await mkdir(locked);
    await writeFile(join(locked, "a.txt"), "alpha\n");
    await chmod(locked, 0o000);
    try {
      await assert.rejects(fileStatus(join(locked, "a.txt")), { code: "EACCES" });
    } finally {
      await chmod(locked, 0o700);
    }
  }));
