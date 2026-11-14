// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

import assert from "node:assert/strict";
import { chmodSync, mkdtempSync, readdirSync, readFileSync, rmSync, statSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";
import { removeLines } from "./removeLines.ts";

const root = mkdtempSync(join(tmpdir(), "remove-lines-"));
after(() => rmSync(root, { recursive: true }));

const numbers = Array.from({ length: 100000 }, (_, i) => `${i}\n`).join("");

// [name, the text of the file, start, count, the result, the text after]
const cases: [string, string, number, number, number, string][] = [
  ["first-two", "a\nb\nc\nd\n", 1, 2, 2, "c\nd\n"],
  ["middle", "a\nb\nc\nd\n", 2, 2, 2, "a\nd\n"],
  ["last", "a\nb\nc\n", 3, 1, 1, "a\nb\n"],
  ["past-the-end", "a\nb\n", 3, 1, 0, "a\nb\n"],
  ["partly-past-the-end", "a\nb\nc\n", 2, 5, 2, "a\n"],
  ["count-zero", "a\nb\n", 1, 0, 0, "a\nb\n"],
  ["all-lines", "a\nb\n", 1, 2, 2, ""],
  ["empty", "", 1, 1, 0, ""],
  ["empty-line", "a\n\nb\n", 2, 1, 1, "a\nb\n"],
  ["no-final-newline", "a\nb", 1, 1, 1, "b"],
  ["remove-incomplete-line", "a\nb", 2, 1, 1, "a\n"],
  ["crlf", "a\r\nb\r\nc\r\n", 2, 1, 1, "a\r\nc\r\n"],
  ["lone-cr", "a\rb\nc\n", 1, 1, 1, "c\n"],
  ["non-ascii", "caf\u00e9\nna\u00efve\nzo\u00eb\n", 2, 1, 1, "caf\u00e9\nzo\u00eb\n"],
  ["large-count", "a\nb\n", 2, 2147483647, 1, "a\n"],
  // 100000 lines with the numbers 0 to 99999. Lines 2 to 99999 go.
  ["many-lines", numbers, 2, 99998, 99998, "0\n99999\n"],
];

for (const [name, text, start, count, want, afterText] of cases) {
  test(name, () => {
    const dir = mkdtempSync(join(root, "case-"));
    const path = join(dir, "input.txt");
    writeFileSync(path, text, "utf8");
    assert.equal(removeLines(path, start, count), want);
    assert.equal(readFileSync(path, "utf8"), afterText);
    // No temporary file stays in the directory.
    assert.deepEqual(readdirSync(dir), ["input.txt"]);
  });
}

test("keeps-the-permissions", { skip: process.platform === "win32" }, () => {
  const path = join(mkdtempSync(join(root, "case-")), "input.txt");
  writeFileSync(path, "a\nb\n");
  chmodSync(path, 0o640);
  assert.equal(removeLines(path, 1, 1), 1);
  assert.equal(statSync(path).mode & 0o777, 0o640);
});

test("not-found", () => {
  assert.throws(() => removeLines(join(root, "missing.txt"), 1, 1), { code: "ENOENT" });
});

for (const [name, start, count] of [["start-zero", 0, 1], ["negative-count", 1, -1]] as const) {
  test(name, () => {
    const path = join(mkdtempSync(join(root, "case-")), "input.txt");
    writeFileSync(path, "a\nb\n");
    assert.throws(() => removeLines(path, start, count), RangeError);
    assert.equal(readFileSync(path, "utf8"), "a\nb\n");
  });
}
