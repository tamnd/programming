// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { countLines } from "./countLines.ts";

// name: [the bytes of the file, newlines, lines]
const CASES: Record<string, [string | Buffer, number, number]> = {
  lf: ["alpha\nbeta\ngamma\n", 3, 3],
  "blank-lines": ["first\n\n\nlast\n", 4, 4],
  empty: ["", 0, 0],
  "only-newline": ["\n", 1, 1],
  "no-final-newline": ["one\ntwo\nthree", 2, 3],
  "one-line-no-newline": ["x", 0, 1],
  crlf: ["one\r\ntwo\r\n", 2, 2],
  "crlf-no-final-newline": ["one\r\ntwo", 1, 2],
  "cr-only": ["one\rtwo\r", 0, 1],
  "other-breaks": [Buffer.from("a\vb\fc\u0085d\u2028e\n"), 1, 1],
  "invalid-utf8": [Buffer.from([0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a]), 2, 2],
  "nul-bytes": ["a\0b\n\0\n", 2, 2],
  // 100000 lines and a last line with no LF: more than one chunk of 64 KiB.
  "many-chunks": ["x\n".repeat(100000) + "end", 100000, 100001],
};

const dir = mkdtempSync(join(tmpdir(), "count-lines-"));
after(() => rmSync(dir, { recursive: true, force: true }));

for (const [name, [data, newlines, lines]] of Object.entries(CASES)) {
  test(name, async () => {
    const path = join(dir, `${name}.txt`);
    writeFileSync(path, data);
    assert.deepEqual(await countLines(path), { newlines, lines });
  });
}

test("missing file", async () => {
  await assert.rejects(countLines(join(dir, "missing.txt")), { code: "ENOENT" });
});
