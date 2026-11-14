// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";
import { filterLines } from "./filterLines.ts";

const dir = mkdtempSync(join(tmpdir(), "filter-lines-"));
after(() => rmSync(dir, { recursive: true }));

const numbers = Array.from({ length: 100000 }, (_, i) => `${i}\n`).join("");

// [name, the text of the file, the pattern, the result]
const cases: [string, string, string, string[]][] = [
  ["literal", "apple\nbanana\ncherry\n", "an", ["banana"]],
  ["several", "cat\ndog\ncatalog\n", "cat", ["cat", "catalog"]],
  ["start", "ab\nba\n", "^a", ["ab"]],
  ["end", "ab\nba\n", "a$", ["ba"]],
  ["dot", "abc\nac\naxc\n", "a.c", ["abc", "axc"]],
  ["star", "ac\nabc\nabbc\nadc\n", "ab*c", ["ac", "abc", "abbc"]],
  ["whole-line", "a\nab\nba\n", "^a$", ["a"]],
  ["empty", "", "a", []],
  ["no-match", "abc\n", "x", []],
  ["empty-lines", "a\n\nb\n\n", "^$", ["", ""]],
  ["match-all", "a\n\nb\n", ".*", ["a", "", "b"]],
  ["duplicates", "a\nb\na\n", "a", ["a", "a"]],
  ["no-final-newline", "a\nab", "b", ["ab"]],
  ["crlf-end", "ab\r\nab\n", "b$", ["ab"]],
  ["crlf-dot", "a\r\n", "^a.$", ["a\r"]],
  ["case", "Apple\napple\n", "apple", ["apple"]],
  ["non-ascii", "caf\u00e9\ncafe\n", "\u00e9", ["caf\u00e9"]],
  // 100000 lines with the numbers 0 to 99999. Five of them hold only nines.
  ["many-lines", numbers, "^9*$", ["9", "99", "999", "9999", "99999"]],
];

for (const [name, text, pattern, want] of cases) {
  test(name, () => {
    const path = join(dir, `${name}.txt`);
    writeFileSync(path, text, "utf8");
    assert.deepEqual(filterLines(path, pattern), want);
  });
}

test("not-found", () => {
  assert.throws(() => filterLines(join(dir, "missing.txt"), "a"), { code: "ENOENT" });
});
