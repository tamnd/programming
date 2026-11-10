// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

import assert from "node:assert/strict";
import { mkdir, mkdtemp, readdir, readFile, rm, stat, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { test } from "node:test";

import { writeText } from "./writeText.js";

// [case, text, mode, the file before the call or null, the file after the call]
const cases = [
  ["new-file", "alpha\nbeta\n", "truncate", null, "alpha\nbeta\n"],
  ["append-existing", "two\n", "append", "one\n", "one\ntwo\n"],
  ["atomic-replace", "new\n", "atomic", "old\n", "new\n"],
  ["truncate-longer", "short\n", "truncate", "a much longer old line\n", "short\n"],
  ["empty-truncate", "", "truncate", "old\n", ""],
  ["empty-new", "", "truncate", null, ""],
  ["append-missing", "first\n", "append", null, "first\n"],
  ["append-no-newline", "two", "append", "one", "onetwo"],
  ["non-ascii", "naïve café\n日本語\n🙂\n", "truncate", null, "naïve café\n日本語\n🙂\n"],
  ["crlf", "one\r\ntwo\r\n", "truncate", null, "one\r\ntwo\r\n"],
  ["mixed-endings", "lf\ncrlf\r\ncr\rend", "truncate", null, "lf\ncrlf\r\ncr\rend"],
  ["backslashes", "C:\\temp\\new\n", "truncate", null, "C:\\temp\\new\n"],
  ["format-chars", "-n 100% %s\n", "truncate", null, "-n 100% %s\n"],
  ["atomic-new", "new\n", "atomic", null, "new\n"],
  ["atomic-empty", "", "atomic", "old\n", ""],
  ["atomic-non-ascii", "naïve café\n日本語\n🙂\n", "atomic", "old\n", "naïve café\n日本語\n🙂\n"],
];

async function withDir(body) {
  const dir = await mkdtemp(join(tmpdir(), "write-text-"));
  try {
    await body(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

for (const [name, text, mode, before, want] of cases) {
  test(name, () =>
    withDir(async (dir) => {
      const file = join(dir, "out.txt");
      if (before !== null) await writeFile(file, Buffer.from(before, "utf8"));
      await writeText(file, text, mode);
      // The exact bytes: UTF-8, no byte order mark, no change to the line ends.
      assert.deepEqual(await readFile(file), Buffer.from(want, "utf8"));
      assert.deepEqual(await readdir(dir), ["out.txt"]);
    }),
  );
}

for (const mode of ["truncate", "append", "atomic"]) {
  test(`missing-dir-${mode}`, () =>
    withDir(async (dir) => {
      await assert.rejects(writeText(join(dir, "missing", "out.txt"), "x\n", mode), { code: "ENOENT" });
      assert.deepEqual(await readdir(dir), []);
    }),
  );
}

test("atomic-onto-directory", () =>
  withDir(async (dir) => {
    const file = join(dir, "out.txt");
    await mkdir(file);
    await assert.rejects(writeText(file, "x\n", "atomic"));
    assert.ok((await stat(file)).isDirectory());
    assert.deepEqual(await readdir(dir), ["out.txt"]);
  }),
);
