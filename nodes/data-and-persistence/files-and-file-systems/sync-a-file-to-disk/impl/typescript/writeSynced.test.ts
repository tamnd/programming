// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

import assert from "node:assert/strict";
import { mkdirSync, mkdtempSync, readdirSync, readFileSync, rmSync, statSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { test } from "node:test";

import { syncDirectory, writeSynced } from "./writeSynced.ts";

// [case, text, the file before the call or null]
const cases: [string, string, string | null][] = [
  ["new-file", "alpha\nbeta\n", null],
  ["replace-longer", "short\n", "a much longer old line\n"],
  ["many-lines", "one\ntwo\nthree\nfour\n", null],
  ["empty-replace", "", "old\n"],
  ["empty-new", "", null],
  ["no-final-newline", "last line", null],
  ["non-ascii", "naïve café\n日本語\n🙂\n", null],
  ["crlf", "one\r\ntwo\r\n", null],
];

function withDir(fn: (dir: string) => void): void {
  const dir = mkdtempSync(join(tmpdir(), "sync-"));
  try {
    fn(dir);
  } finally {
    rmSync(dir, { recursive: true, force: true });
  }
}

for (const [name, text, before] of cases) {
  test(name, () =>
    withDir((dir) => {
      const file = join(dir, "out.txt");
      if (before !== null) writeFileSync(file, before);
      writeSynced(file, text);
      assert.deepEqual(readFileSync(file), Buffer.from(text, "utf8"));
      assert.deepEqual(readdirSync(dir), ["out.txt"]);
    }),
  );
}

test("sync-directory", () => withDir((dir) => syncDirectory(dir)));

test("missing-directory", () =>
  withDir((dir) => {
    assert.throws(() => writeSynced(join(dir, "missing", "out.txt"), "x\n"), { code: "ENOENT" });
    assert.deepEqual(readdirSync(dir), []);
  }),
);

test("onto-directory", () =>
  withDir((dir) => {
    const file = join(dir, "out.txt");
    mkdirSync(file);
    assert.throws(() => writeSynced(file, "x\n"), { code: "EISDIR" });
    assert.ok(statSync(file).isDirectory());
  }),
);
