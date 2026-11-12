// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { access, mkdtemp, readFile, rm, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { appendLine } from "./appendLine.ts";

// name: [old content or null, line, content after]
const cases: Record<string, [string | null, string, string]> = {
  "new-file": [null, "first", "first\n"],
  "append-existing": ["one\n", "two", "one\ntwo\n"],
  "new-file-empty-line": [null, "", "\n"],
  "empty-line": ["a\n", "", "a\n\n"],
  "empty-file": ["", "x", "x\n"],
  "no-final-newline": ["one", "two", "onetwo\n"],
  "crlf-file": ["one\r\n", "two", "one\r\ntwo\n"],
  whitespace: ["", "  two spaces\ttab ", "  two spaces\ttab \n"],
  "non-ascii": ["naïve\n", "café 日本語 🙂", "naïve\ncafé 日本語 🙂\n"],
};

async function withDir(body: (dir: string) => Promise<void>): Promise<void> {
  const dir = await mkdtemp(join(tmpdir(), "append-line-"));
  try {
    await body(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

for (const [name, [old, line, want]] of Object.entries(cases)) {
  test(name, () =>
    withDir(async (dir) => {
      const path = join(dir, "log.txt");
      if (old !== null) await writeFile(path, old, "utf8");
      await appendLine(path, line);
      assert.equal(await readFile(path, "utf8"), want);
    }));
}

test("missing directory", () =>
  withDir(async (dir) => {
    await assert.rejects(appendLine(join(dir, "missing", "log.txt"), "x"), { code: "ENOENT" });
  }));

for (const line of ["a\nb", "a\rb"]) {
  test(`invalid line ${JSON.stringify(line)}`, () =>
    withDir(async (dir) => {
      const path = join(dir, "log.txt");
      await assert.rejects(appendLine(path, line), RangeError);
      await assert.rejects(access(path), { code: "ENOENT" });
    }));
}

test("many writers", () =>
  withDir(async (dir) => {
    const path = join(dir, "log.txt");
    const writers = 8;
    const lines = 250;
    const text = (w: number, i: number) => `writer ${w} line ${String(i).padStart(3, "0")} ` + "x".repeat(40);
    await Promise.all(
      Array.from({ length: writers }, async (_, w) => {
        for (let i = 0; i < lines; i++) await appendLine(path, text(w, i));
      }),
    );
    const got = (await readFile(path, "utf8")).split("\n");
    assert.equal(got.pop(), "");
    const want: string[] = [];
    for (let w = 0; w < writers; w++) for (let i = 0; i < lines; i++) want.push(text(w, i));
    assert.deepEqual(got.sort(), want.sort());
  }));
