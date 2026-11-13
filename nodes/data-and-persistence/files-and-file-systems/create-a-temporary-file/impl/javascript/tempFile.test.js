// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

import assert from "node:assert/strict";
import { lstat, mkdir, mkdtemp, readdir, rm } from "node:fs/promises";
import { tmpdir } from "node:os";
import { basename, dirname, join } from "node:path";
import { afterEach, beforeEach, test } from "node:test";

import { makeTempFile, useTempFile } from "./tempFile.js";

const cases = [
  ["one-line", ".", "tmp-", "alpha\n"],
  ["many-lines", ".", "report-", "alpha\nbeta\ngamma\n"],
  ["in-subdirectory", "sub", "tmp-", "alpha\n"],
  ["empty-prefix", ".", "", "alpha\n"],
  ["empty-data", ".", "tmp-", ""],
  ["non-ascii-data", ".", "tmp-", "café 日本\n"],
  ["prefix-with-spaces", ".", "my temp ", "alpha\n"],
  ["non-ascii-prefix", ".", "café-", "alpha\n"],
];

let dir;

beforeEach(async () => {
  dir = await mkdtemp(join(tmpdir(), "temp-file-"));
  await mkdir(join(dir, "sub"));
});

afterEach(async () => {
  await rm(dir, { recursive: true, force: true });
});

for (const [name, rel, prefix, data] of cases) {
  test(name, async () => {
    assert.equal(await useTempFile(join(dir, rel), prefix, data), data);
    assert.deepEqual(await readdir(dir), ["sub"]);
    assert.deepEqual(await readdir(join(dir, "sub")), []);
  });
}

for (const prefix of ["tmp-", "", "my temp ", "café-"]) {
  test(`make-temp-file ${JSON.stringify(prefix)}`, async () => {
    const path = await makeTempFile(dir, prefix);
    assert.equal(dirname(path), dir);
    assert.ok(basename(path).startsWith(prefix));
    assert.ok(basename(path).length >= prefix.length + 6);
    const st = await lstat(path);
    assert.ok(st.isFile());
    assert.equal(st.mode & 0o7777, 0o600);
    assert.equal(st.size, 0);
  });
}

test("two-calls-give-two-files", async () => {
  const a = await makeTempFile(dir, "tmp-");
  const b = await makeTempFile(dir, "tmp-");
  assert.notEqual(a, b);
  assert.equal((await readdir(dir)).length, 3);
});

test("missing-directory", async () => {
  await assert.rejects(useTempFile(join(dir, "nodir"), "tmp-", "alpha\n"), { code: "ENOENT" });
});

test("prefix-with-slash", async () => {
  await assert.rejects(useTempFile(dir, "sub/tmp-", "alpha\n"), RangeError);
  assert.deepEqual(await readdir(join(dir, "sub")), []);
});
