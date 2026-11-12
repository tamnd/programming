// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { existsSync } from "node:fs";
import { mkdtemp, readFile, rm, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { copyBuffered } from "./copyBuffered.js";

const text = Buffer.from("alpha\nbeta\ngamma\n");
const allBytes = Buffer.from(Array.from({ length: 256 }, (_, i) => i));
const large = Buffer.concat(Array(4096).fill(allBytes));

const cases = {
  text: [text, 4096],
  "several-buffers": [text, 4],
  empty: [Buffer.alloc(0), 8],
  "smaller-than-buffer": [Buffer.from("12345"), 8],
  "exact-buffer": [Buffer.from("12345678"), 8],
  "one-past-buffer": [Buffer.from("123456789"), 8],
  "size-one": [text, 1],
  binary: [allBytes, 64],
  large: [large, 4096],
  "large-odd-buffer": [large, 4093],
};

async function withDir(fn) {
  const dir = await mkdtemp(join(tmpdir(), "copy-buffered-"));
  try {
    await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

for (const [name, [data, size]] of Object.entries(cases)) {
  test(name, () =>
    withDir(async (dir) => {
      const src = join(dir, "in.bin");
      const dst = join(dir, "out.bin");
      await writeFile(src, data);
      assert.equal(await copyBuffered(src, dst, size), data.length);
      assert.deepEqual(await readFile(dst), data);
    }));
}

test("replaces a longer file", () =>
  withDir(async (dir) => {
    const src = join(dir, "in.bin");
    const dst = join(dir, "out.bin");
    await writeFile(src, "new");
    await writeFile(dst, "old and longer");
    assert.equal(await copyBuffered(src, dst, 8), 3);
    assert.equal(await readFile(dst, "utf8"), "new");
  }));

test("missing src", () =>
  withDir(async (dir) => {
    await assert.rejects(copyBuffered(join(dir, "missing.bin"), join(dir, "out.bin"), 8), { code: "ENOENT" });
    assert.equal(existsSync(join(dir, "out.bin")), false);
  }));

test("missing dst dir", () =>
  withDir(async (dir) => {
    const src = join(dir, "in.bin");
    await writeFile(src, text);
    await assert.rejects(copyBuffered(src, join(dir, "no-such-dir", "out.bin"), 8), { code: "ENOENT" });
  }));

for (const size of [0, -1, 1.5]) {
  test(`invalid size ${size}`, () =>
    withDir(async (dir) => {
      const src = join(dir, "in.bin");
      await writeFile(src, text);
      await assert.rejects(copyBuffered(src, join(dir, "out.bin"), size), RangeError);
    }));
}

for (const [name, data] of [["full device, small", text], ["full device, large", large]]) {
  test(name, { skip: !existsSync("/dev/full") && "no /dev/full" }, () =>
    withDir(async (dir) => {
      const src = join(dir, "in.bin");
      await writeFile(src, data);
      await assert.rejects(copyBuffered(src, "/dev/full", 4096), { code: "ENOSPC" });
    }));
}
