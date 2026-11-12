// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { mkdtemp, rm, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { readText } from "./readText.ts";

const cases: Record<string, string> = {
  lf: "alpha\nbeta\n",
  paragraphs: "first line\n\nsecond paragraph\n",
  "non-ascii": "naïve café\n日本語\n🙂\n",
  empty: "",
  "no-final-newline": "one\ntwo",
  "trailing-newlines": "end\n\n\n",
  crlf: "one\r\ntwo\r\n",
  "mixed-endings": "lf\ncrlf\r\ncr\rend",
  bom: "\uFEFFhead\n",
  whitespace: "  two spaces\ttab \n",
  large: "0123456789abcdef".repeat(65536),
};

const invalidUtf8: Record<string, number[]> = {
  "invalid-byte": [0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a],
  "truncated-at-end": [0x6f, 0x6b, 0x0a, 0x63, 0x61, 0x66, 0xc3],
  "encoded-surrogate": [0x61, 0xed, 0xa0, 0x80, 0x62, 0x0a],
  overlong: [0xc0, 0xaf, 0x0a],
};

async function withDir(body: (dir: string) => Promise<void>): Promise<void> {
  const dir = await mkdtemp(join(tmpdir(), "read-text-"));
  try {
    await body(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

for (const [name, text] of Object.entries(cases)) {
  test(name, () =>
    withDir(async (dir) => {
      const path = join(dir, "input.txt");
      await writeFile(path, text, "utf8");
      assert.equal(await readText(path), text);
    }));
}

test("missing file", () =>
  withDir(async (dir) => {
    await assert.rejects(readText(join(dir, "missing.txt")), { code: "ENOENT" });
  }));

test("directory", () =>
  withDir(async (dir) => {
    await assert.rejects(readText(dir), { code: "EISDIR" });
  }));

for (const [name, bytes] of Object.entries(invalidUtf8)) {
  test(name, () =>
    withDir(async (dir) => {
      const path = join(dir, "input.txt");
      await writeFile(path, Uint8Array.from(bytes));
      await assert.rejects(readText(path), TypeError);
    }));
}
