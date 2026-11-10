// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

import assert from "node:assert/strict";
import { mkdtempSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { test } from "node:test";

import { lineStats } from "./lineStats.ts";

const dir = mkdtempSync(join(tmpdir(), "line-stats-"));

function write(name: string, data: string | Uint8Array): string {
  const path = join(dir, name);
  writeFileSync(path, data);
  return path;
}

const cases: [string, string, number, number][] = [
  ["lf", "alpha\nbeta\ngamma\n", 3, 5],
  ["blank-lines", "first\n\n\nlast line\n", 4, 9],
  ["empty", "", 0, 0],
  ["only-newline", "\n", 1, 0],
  ["no-final-newline", "one\ntwo\nthree", 3, 5],
  ["crlf", "one\r\ntwo\r\nthree\r\n", 3, 5],
  ["crlf-no-final-newline", "one\r\ntwo\r\nthree", 3, 5],
  ["mixed-endings", "one\r\ntwo\nthree\r\n", 3, 5],
  ["whitespace", "  two spaces\ttab \n", 1, 17],
  ["backslashes", "C:\\temp\\new\n", 1, 11],
  ["long-line", "x".repeat(1 << 20) + "\nend\n", 2, 1 << 20],
  ["non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", 3, 10],
];

for (const [name, data, lines, longest] of cases) {
  test(name, async () => {
    assert.deepEqual(await lineStats(write(name, data)), { lines, longest });
  });
}

test("missing", async () => {
  await assert.rejects(lineStats(join(dir, "missing.txt")), { code: "ENOENT" });
});

const invalid: [string, number[]][] = [
  ["invalid-byte", [0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a]],
  ["truncated-at-end", [0x6f, 0x6b, 0x0a, 0x63, 0x61, 0x66, 0xc3]],
  ["encoded-surrogate", [0x61, 0xed, 0xa0, 0x80, 0x62, 0x0a]],
  ["overlong", [0xc0, 0xaf, 0x0a]],
];

for (const [name, bytes] of invalid) {
  test(name, async () => {
    const path = write(name, Uint8Array.from(bytes));
    await assert.rejects(lineStats(path), { code: "ERR_ENCODING_INVALID_ENCODED_DATA" });
  });
}
