// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { existsSync, lstatSync, statSync } from "node:fs";
import { mkdir, mkdtemp, open, readFile, readlink, rm, symlink, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { dirname, join } from "node:path";
import { renameFile } from "./renameFile.js";

async function withDir(fn) {
  const dir = await mkdtemp(join(tmpdir(), "rename-file-"));
  try {
    await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

function lexists(path) {
  try {
    lstatSync(path);
    return true;
  } catch {
    return false;
  }
}

// name: [src, dst, content of src, content of dst or null, content of dst after]
const cases = {
  rename: ["a.txt", "b.txt", "alpha\n", null, "alpha\n"],
  "replace-existing": ["a.txt", "b.txt", "alpha\n", "beta\n", "alpha\n"],
  "same-path": ["a.txt", "a.txt", "alpha\n", "alpha\n", "alpha\n"],
  "empty-file": ["a.txt", "b.txt", "", "beta\n", ""],
  "other-directory": ["a.txt", "sub/a.txt", "alpha\n", null, "alpha\n"],
  "name-with-spaces": ["my notes.txt", "old notes.txt", "alpha\n", null, "alpha\n"],
  "non-ascii-name": ["a.txt", "café 日本.txt", "alpha\n", null, "alpha\n"],
};

for (const [name, [srcRel, dstRel, srcContent, dstContent, want]] of Object.entries(cases)) {
  test(name, () =>
    withDir(async (dir) => {
      const src = join(dir, srcRel);
      const dst = join(dir, dstRel);
      await mkdir(dirname(dst), { recursive: true });
      if (dstContent !== null) await writeFile(dst, dstContent);
      await writeFile(src, srcContent);
      await renameFile(src, dst);
      assert.equal(await readFile(dst, "utf8"), want);
      assert.equal(src === dst || !lexists(src), true);
    }));
}

test("missing src", () =>
  withDir(async (dir) => {
    await assert.rejects(renameFile(join(dir, "a.txt"), join(dir, "b.txt")), { code: "ENOENT" });
  }));

test("missing dst dir", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await assert.rejects(renameFile(join(dir, "a.txt"), join(dir, "no-such-dir", "b.txt")), { code: "ENOENT" });
    assert.equal(await readFile(join(dir, "a.txt"), "utf8"), "alpha\n");
  }));

test("dst is a directory", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await mkdir(join(dir, "dir"));
    await assert.rejects(renameFile(join(dir, "a.txt"), join(dir, "dir")));
    assert.equal(lstatSync(join(dir, "dir")).isDirectory(), true);
    assert.equal(await readFile(join(dir, "a.txt"), "utf8"), "alpha\n");
  }));

test("link", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await symlink("a.txt", join(dir, "link"));
    await renameFile(join(dir, "link"), join(dir, "new-link"));
    assert.equal(await readlink(join(dir, "new-link")), "a.txt");
    assert.equal(lexists(join(dir, "link")), false);
  }));

test("an open dst keeps the old data", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await writeFile(join(dir, "b.txt"), "beta\n");
    const old = await open(join(dir, "b.txt"));
    try {
      await renameFile(join(dir, "a.txt"), join(dir, "b.txt"));
      assert.equal(await old.readFile("utf8"), "beta\n");
    } finally {
      await old.close();
    }
    assert.equal(await readFile(join(dir, "b.txt"), "utf8"), "alpha\n");
  }));

test("cross device", (t) =>
  withDir(async (dir) => {
    if (!existsSync("/dev/shm") || statSync("/dev/shm").dev === statSync(dir).dev) {
      t.skip("no other file system at /dev/shm");
      return;
    }
    await writeFile(join(dir, "a.txt"), "alpha\n");
    const dst = join("/dev/shm", `rename-file-${process.pid}.txt`);
    await assert.rejects(renameFile(join(dir, "a.txt"), dst), { code: "EXDEV" });
    assert.equal(await readFile(join(dir, "a.txt"), "utf8"), "alpha\n");
    assert.equal(existsSync(dst), false);
  }));
