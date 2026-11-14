// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";
import { firstAndLastLines } from "./firstAndLastLines.js";

const dir = mkdtempSync(join(tmpdir(), "first-and-last-"));
after(() => rmSync(dir, { recursive: true }));

const lines = (from, to) => Array.from({ length: to - from }, (_, i) => `${from + i}`);
const numbers = lines(0, 100000).map((s) => `${s}\n`).join("");

// [name, the text of the file, the count, the first lines, the last lines]
const cases = [
  ["two", "a\nb\nc\nd\ne\n", 2, ["a", "b"], ["d", "e"]],
  ["one", "first\nmiddle\nlast\n", 1, ["first"], ["last"]],
  ["ten", lines(1, 13).map((s) => `${s}\n`).join(""), 10, lines(1, 11), lines(3, 13)],
  ["empty", "", 2, [], []],
  ["zero", "a\nb\n", 0, [], []],
  ["short", "a\nb\n", 5, ["a", "b"], ["a", "b"]],
  ["exact", "a\nb\n", 2, ["a", "b"], ["a", "b"]],
  ["overlap", "a\nb\nc\n", 2, ["a", "b"], ["b", "c"]],
  ["one-line", "a\n", 1, ["a"], ["a"]],
  ["empty-lines", "\n\na\n\n", 1, [""], [""]],
  ["only-newline", "\n", 1, [""], [""]],
  ["no-final-newline", "a\nb\nc", 1, ["a"], ["c"]],
  ["crlf", "a\r\nb\r\n", 1, ["a\r"], ["b\r"]],
  ["non-ascii", "caf\u00e9\nna\u00efve\nzo\u00eb\n", 1, ["caf\u00e9"], ["zo\u00eb"]],
  // 100000 lines with the numbers 0 to 99999.
  ["many-lines", numbers, 3, ["0", "1", "2"], ["99997", "99998", "99999"]],
];

for (const [name, text, count, first, last] of cases) {
  test(name, () => {
    const path = join(dir, `${name}.txt`);
    writeFileSync(path, text, "utf8");
    assert.deepEqual(firstAndLastLines(path, count), [first, last]);
  });
}

test("not-found", () => {
  assert.throws(() => firstAndLastLines(join(dir, "missing.txt"), 1), { code: "ENOENT" });
});
