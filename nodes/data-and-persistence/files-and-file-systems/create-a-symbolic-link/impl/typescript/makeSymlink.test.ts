// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

import assert from "node:assert/strict";
import { lstat, mkdir, mkdtemp, readFile, readlink, rm, symlink, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { afterEach, beforeEach, test } from "node:test";

import { makeSymlink } from "./makeSymlink.ts";

const cases = [
  ["file-target", "a.txt", "link"],
  ["directory-target", "sub", "link"],
  ["link-in-subdirectory", "b.txt", "sub/link"],
  ["dangling-target", "missing.txt", "link"],
  ["relative-target", "a.txt", "sub/link"],
  ["parent-target", "../a.txt", "sub/link"],
  ["name-with-spaces", "my notes.txt", "my link"],
  ["non-ascii-name", "café 日本.txt", "lien 日本"],
];

let dir: string;

beforeEach(async () => {
  dir = await mkdtemp(join(tmpdir(), "make-symlink-"));
  await writeFile(join(dir, "a.txt"), "alpha\n");
  await mkdir(join(dir, "sub"));
  await writeFile(join(dir, "sub", "b.txt"), "beta\n");
});

afterEach(async () => {
  await rm(dir, { recursive: true, force: true });
});

for (const [name, target, rel] of cases) {
  test(name, async () => {
    assert.equal(await makeSymlink(target, join(dir, rel)), target);
    assert.ok((await lstat(join(dir, rel))).isSymbolicLink());
  });
}

test("file and directory exist", async () => {
  await assert.rejects(makeSymlink("b.txt", join(dir, "a.txt")), { code: "EEXIST" });
  await assert.rejects(makeSymlink("a.txt", join(dir, "sub")), { code: "EEXIST" });
});

test("dangling link exists", async () => {
  await symlink("missing.txt", join(dir, "link"));
  await assert.rejects(makeSymlink("a.txt", join(dir, "link")), { code: "EEXIST" });
  assert.equal(await readlink(join(dir, "link")), "missing.txt");
});

test("missing directory", async () => {
  await assert.rejects(makeSymlink("a.txt", join(dir, "nodir", "link")), { code: "ENOENT" });
});

test("link reads target", async () => {
  await makeSymlink("a.txt", join(dir, "link"));
  assert.equal(await readFile(join(dir, "link"), "utf8"), "alpha\n");
  await makeSymlink("../a.txt", join(dir, "sub", "up"));
  assert.equal(await readFile(join(dir, "sub", "up"), "utf8"), "alpha\n");
});

test("absolute target", async () => {
  const target = join(dir, "a.txt");
  assert.equal(await makeSymlink(target, join(dir, "link")), target);
});
