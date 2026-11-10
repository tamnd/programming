// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

import assert from "node:assert/strict";
import { spawnSync } from "node:child_process";
import { Readable } from "node:stream";
import { test } from "node:test";

import { readLines } from "./readLines.ts";

const long = "x".repeat(1 << 20);

const cases: [string, string, string[]][] = [
  ["lf", "alpha\nbeta\ngamma\n", ["alpha", "beta", "gamma"]],
  ["blank-lines", "first\n\n\nlast line\n", ["first", "", "", "last line"]],
  ["empty", "", []],
  ["only-newline", "\n", [""]],
  ["no-final-newline", "one\ntwo\nthree", ["one", "two", "three"]],
  ["crlf", "one\r\ntwo\r\nthree\r\n", ["one", "two", "three"]],
  ["crlf-no-final-newline", "one\r\ntwo\r\nthree", ["one", "two", "three"]],
  ["mixed-endings", "one\r\ntwo\nthree\r\n", ["one", "two", "three"]],
  ["trailing-blank-line", "last\n\n", ["last", ""]],
  ["whitespace", "  two spaces\ttab \n", ["  two spaces\ttab "]],
  ["backslashes", "C:\\temp\\new\n", ["C:\\temp\\new"]],
  [
    "non-ascii",
    "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n",
    ["naïve café", "日本語のテキスト", "🙂🙂🙂🙂🙂🙂"],
  ],
  ["long-line", long + "\nend\n", [long, "end"]],
];

// The child process reads its real standard input and prints the result as JSON.
const modulePath = new URL("./readLines.ts", import.meta.url).href;
const child = `const { readLines } = await import(${JSON.stringify(modulePath)});
process.stdout.write(JSON.stringify(await readLines()));`;

for (const [name, data, want] of cases) {
  test(name, () => {
    const result = spawnSync(process.execPath, ["--input-type=module", "-e", child], {
      input: data,
      encoding: "utf8",
      maxBuffer: 4 << 20,
    });
    assert.equal(result.status, 0, result.stderr);
    assert.deepEqual(JSON.parse(result.stdout), want);
  });
}

test("chunk-boundaries", async () => {
  // One chunk ends between "\r" and "\n", and one ends inside the 2 bytes of "é".
  const bytes = Buffer.from("one\r\ncafé\n");
  const chunks = [bytes.subarray(0, 4), bytes.subarray(4, 9), bytes.subarray(9)];
  const input = Readable.from(chunks, { objectMode: false });
  assert.deepEqual(await readLines(input), ["one", "café"]);
});
