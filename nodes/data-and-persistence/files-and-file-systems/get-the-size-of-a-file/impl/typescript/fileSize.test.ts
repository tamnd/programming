// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { mkdir, mkdtemp, open, rm, symlink, truncate, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { fileSize } from "./fileSize.ts";

async function withDir(fn: (dir: string) => Promise<void>): Promise<void> {
  const dir = await mkdtemp(join(tmpdir(), "file-size-"));
  try {
    await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

const cases: [string, string, Buffer, number][] = [
  ["text", "a.txt", Buffer.from("alpha\n"), 6],
  ["non-ascii-text", "a.txt", Buffer.from("café\n"), 6],
  ["empty-file", "a.txt", Buffer.alloc(0), 0],
  ["binary-data", "a.bin", Buffer.from([0x00, 0x0d, 0x0a, 0xc3, 0xbf, 0x00]), 6],
  ["crlf-line-ends", "a.txt", Buffer.from("a\r\nb\r\n"), 6],
  ["name-with-spaces", "my notes.txt", Buffer.from("alpha\n"), 6],
  ["non-ascii-name", "café 日本.txt", Buffer.from("alpha\n"), 6],
];

for (const [name, rel, data, size] of cases) {
  test(name, () =>
    withDir(async (dir) => {
      await writeFile(join(dir, rel), data);
      assert.equal(await fileSize(join(dir, rel)), size);
    }),
  );
}

test("errors", () =>
  withDir(async (dir) => {
    await assert.rejects(fileSize(join(dir, "a.txt")), { code: "ENOENT" });
    await mkdir(join(dir, "dir"));
    await assert.rejects(fileSize(join(dir, "dir")), { code: "EISDIR" });
  }));

test("symbolic link", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "a.txt"), "alpha beta\n");
    await symlink("a.txt", join(dir, "link"));
    assert.equal(await fileSize(join(dir, "link")), 11);
  }));

test("sparse file of 5 GiB", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "sparse.bin"), "");
    await truncate(join(dir, "sparse.bin"), 5 * 1024 ** 3);
    assert.equal(await fileSize(join(dir, "sparse.bin")), 5368709120);
  }));

test("open file", () =>
  withDir(async (dir) => {
    const path = join(dir, "app.log");
    const f = await open(path, "w");
    try {
      await f.write("alpha\n");
      assert.equal(await fileSize(path), 6);
    } finally {
      await f.close();
    }
  }));
