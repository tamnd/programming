// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { chmod, mkdir, mkdtemp, readFile, rm, stat, symlink, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { setMode } from "./setMode.js";

async function withDir(fn) {
  const dir = await mkdtemp(join(tmpdir(), "set-mode-"));
  try {
    await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

async function newFile(path) {
  await writeFile(path, "alpha\n");
  await chmod(path, 0o644);
  return path;
}

const cases = [
  ["owner-read-write-group-read", "a.txt", "640", 0o640],
  ["owner-only", "a.txt", "600", 0o600],
  ["executable", "a.txt", "755", 0o755],
  ["same-mode", "a.txt", "644", 0o644],
  ["no-permissions", "a.txt", "000", 0],
  ["leading-zero", "a.txt", "0640", 0o640],
  ["set-user-id", "a.txt", "4755", 0o4755],
  ["name-with-spaces", "my notes.txt", "640", 0o640],
  ["non-ascii-name", "café 日本.txt", "640", 0o640],
];

for (const [name, rel, mode, result] of cases) {
  test(name, () =>
    withDir(async (dir) => {
      const path = await newFile(join(dir, rel));
      assert.equal(await setMode(path, mode), result);
      assert.equal((await stat(path)).mode & 0o7777, result);
    }),
  );
}

test("invalid modes", () =>
  withDir(async (dir) => {
    const path = await newFile(join(dir, "a.txt"));
    for (const mode of ["648", "64", "10640", "u+x", "", "640\n"]) {
      await assert.rejects(setMode(path, mode), RangeError);
    }
    assert.equal((await stat(path)).mode & 0o7777, 0o644);
  }));

test("missing file", () =>
  withDir(async (dir) => {
    await assert.rejects(setMode(join(dir, "missing.txt"), "640"), { code: "ENOENT" });
  }));

test("a symbolic link changes the target", () =>
  withDir(async (dir) => {
    await newFile(join(dir, "a.txt"));
    await symlink("a.txt", join(dir, "link"));
    assert.equal(await setMode(join(dir, "link"), "600"), 0o600);
    assert.equal((await stat(join(dir, "a.txt"))).mode & 0o777, 0o600);
    assert.equal(await readFile(join(dir, "a.txt"), "utf8"), "alpha\n");
  }));

test("a directory", () =>
  withDir(async (dir) => {
    await mkdir(join(dir, "dir"));
    assert.equal(await setMode(join(dir, "dir"), "700"), 0o700);
  }));

test("the umask does not apply", () =>
  withDir(async (dir) => {
    const path = await newFile(join(dir, "a.txt"));
    const old = process.umask(0o077);
    try {
      assert.equal(await setMode(path, "666"), 0o666);
    } finally {
      process.umask(old);
    }
  }));
