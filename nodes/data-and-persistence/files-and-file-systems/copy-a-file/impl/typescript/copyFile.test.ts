// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { existsSync, lstatSync, statSync } from "node:fs";
import { chmod, link, mkdir, mkdtemp, readdir, readFile, rm, symlink, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { dirname, join } from "node:path";
import { copyFile } from "./copyFile.ts";

async function withDir(fn: (dir: string) => Promise<void>): Promise<void> {
  const dir = await mkdtemp(join(tmpdir(), "copy-file-"));
  try {
    await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

// name: [src, dst, content of src, content of dst or null]
const cases: [string, string, string, string, string | null][] = [
  ["copy", "a.txt", "b.txt", "alpha\n", null],
  ["replace-existing", "a.txt", "b.txt", "alpha\n", "beta, a longer text\n"],
  ["empty-file", "a.txt", "b.txt", "", "beta\n"],
  ["other-directory", "a.txt", "sub/a.txt", "alpha\n", null],
  ["name-with-spaces", "my notes.txt", "copy of my notes.txt", "alpha\n", null],
  ["non-ascii-name", "a.txt", "café 日本.txt", "alpha\n", null],
  ["binary-data", "a.bin", "b.bin", "\0\r\nÿ\0", null],
];

for (const [name, srcRel, dstRel, srcText, dstText] of cases) {
  test(name, () =>
    withDir(async (dir) => {
      const src = join(dir, srcRel);
      const dst = join(dir, dstRel);
      await mkdir(dirname(dst), { recursive: true });
      if (dstText !== null) {
        await writeFile(dst, dstText);
      }
      await writeFile(src, srcText);
      await copyFile(src, dst);
      assert.deepEqual(await readFile(dst), Buffer.from(srcText));
      assert.deepEqual(await readFile(src), Buffer.from(srcText));
    }));
}

test("missing", () =>
  withDir(async (dir) => {
    await assert.rejects(copyFile(join(dir, "a.txt"), join(dir, "b.txt")), { code: "ENOENT" });
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await assert.rejects(copyFile(join(dir, "a.txt"), join(dir, "no-such-dir", "b.txt")), { code: "ENOENT" });
  }));

test("directories", () =>
  withDir(async (dir) => {
    await mkdir(join(dir, "dir"));
    await writeFile(join(dir, "a.txt"), "alpha\n");
    await assert.rejects(copyFile(join(dir, "dir"), join(dir, "b.txt")), { code: "EISDIR" });
    await assert.rejects(copyFile(join(dir, "a.txt"), join(dir, "dir")), { code: "EISDIR" });
    assert.deepEqual(await readdir(join(dir, "dir")), []);
    assert.equal(existsSync(join(dir, "b.txt")), false);
  }));

test("same file", () =>
  withDir(async (dir) => {
    const src = join(dir, "a.txt");
    await writeFile(src, "alpha\n");
    await link(src, join(dir, "b.txt"));
    for (const dst of [src, join(dir, "b.txt")]) {
      await assert.rejects(copyFile(src, dst), { code: "ESAMEFILE" });
      assert.equal(await readFile(src, "utf8"), "alpha\n");
    }
  }));

test("mode", () =>
  withDir(async (dir) => {
    const src = join(dir, "a.sh");
    await writeFile(src, "echo alpha\n");
    await chmod(src, 0o750);
    await copyFile(src, join(dir, "b.sh"));
    assert.equal(statSync(join(dir, "b.sh")).mode & 0o7777, 0o750);
    const other = join(dir, "c.sh");
    await writeFile(other, "beta\n");
    await chmod(other, 0o640);
    await copyFile(src, other);
    assert.equal(statSync(other).mode & 0o7777, 0o750);
  }));

test("large file", () =>
  withDir(async (dir) => {
    const data = Buffer.alloc(5_000_003);
    for (let i = 0; i < data.length; i++) {
      data[i] = (i * 7) % 251;
    }
    await writeFile(join(dir, "big.bin"), data);
    await copyFile(join(dir, "big.bin"), join(dir, "copy.bin"));
    assert.ok(data.equals(await readFile(join(dir, "copy.bin"))));
  }));

test("link at src", () =>
  withDir(async (dir) => {
    await writeFile(join(dir, "target.txt"), "alpha\n");
    await symlink("target.txt", join(dir, "link.txt"));
    await copyFile(join(dir, "link.txt"), join(dir, "b.txt"));
    assert.equal(lstatSync(join(dir, "b.txt")).isSymbolicLink(), false);
    assert.equal(await readFile(join(dir, "b.txt"), "utf8"), "alpha\n");
  }));
