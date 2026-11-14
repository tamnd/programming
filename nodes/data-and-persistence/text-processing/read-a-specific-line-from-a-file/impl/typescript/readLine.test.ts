// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";
import { readLine } from "./readLine.ts";

const dir = mkdtempSync(join(tmpdir(), "read-line-"));
after(() => rmSync(dir, { recursive: true }));

const numbers = Array.from({ length: 100000 }, (_, i) => `${i}\n`).join("");

// [name, the text of the file, the line number, the result]
const cases: [string, string, number, string | null][] = [
  ["first", "a\nb\nc\n", 1, "a"],
  ["middle", "a\nb\nc\n", 2, "b"],
  ["last", "a\nb\nc\n", 3, "c"],
  ["past-the-end", "a\nb\n", 3, null],
  ["zero", "a\n", 0, null],
  ["negative", "a\n", -1, null],
  ["empty", "", 1, null],
  ["empty-line", "a\n\nb\n", 2, ""],
  ["only-newline", "\n", 1, ""],
  ["no-final-newline", "a\nb", 2, "b"],
  ["crlf", "a\r\nb\r\n", 1, "a\r"],
  ["lone-cr", "a\rb\nc\n", 2, "c"],
  ["non-ascii", "caf\u00e9\nzo\u00eb\n", 2, "zo\u00eb"],
  // 100000 lines with the numbers 0 to 99999. Line n holds n - 1.
  ["many-lines", numbers, 100000, "99999"],
  ["many-lines-past-the-end", numbers, 100001, null],
];

for (const [name, text, number, want] of cases) {
  test(name, () => {
    const path = join(dir, `${name}.txt`);
    writeFileSync(path, text, "utf8");
    assert.equal(readLine(path, number), want);
  });
}

test("not-found", () => {
  assert.throws(() => readLine(join(dir, "missing.txt"), 1), { code: "ENOENT" });
});
