// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

import { test } from "node:test";
import assert from "node:assert/strict";
import { mkdtemp, rm, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { readBytes, readExact, UnexpectedEofError } from "./readBytes.js";

const hex = (s) => Buffer.from(s, "hex");
const allBytes = Buffer.from(Array.from({ length: 256 }, (_, i) => i));

const cases = {
  signature: hex("89504e470d0a1a0a0000000d49484452"),
  bytes: hex("0001027f80feff"),
  empty: Buffer.alloc(0),
  "all-bytes": allBytes,
  zeros: Buffer.alloc(8),
  crlf: Buffer.from("a\r\nb\r\n", "latin1"),
  "ctrl-z": hex("611a62"),
  "not-utf8": hex("c328fffe80"),
  large: Buffer.concat(Array(4096).fill(allBytes)),
};

const exact = {
  "signature-only": ["signature", 8, hex("89504e470d0a1a0a")],
  "first-four": ["all-bytes", 4, hex("00010203")],
  "whole-file": ["zeros", 8, Buffer.alloc(8)],
  "zero-count": ["empty", 0, Buffer.alloc(0)],
  "across-ctrl-z": ["ctrl-z", 3, hex("611a62")],
  large: ["large", 1 << 20, cases.large],
};

const short = {
  "one-past-end": ["signature", 17],
  "empty-file": ["empty", 1],
};

async function withFile(data, body) {
  const dir = await mkdtemp(join(tmpdir(), "read-bytes-"));
  try {
    const path = join(dir, "input.bin");
    if (data !== undefined) await writeFile(path, data);
    await body(path, dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

for (const [name, data] of Object.entries(cases)) {
  test(`readBytes ${name}`, () =>
    withFile(data, async (path) => {
      const got = await readBytes(path);
      assert.ok(Buffer.isBuffer(got));
      assert.deepEqual(got, data);
    }));
}

for (const [name, [data, n, want]] of Object.entries(exact)) {
  test(`readExact ${name}`, () =>
    withFile(cases[data], async (path) => {
      assert.deepEqual(await readExact(path, n), want);
    }));
}

for (const [name, [data, n]] of Object.entries(short)) {
  test(`readExact ${name}`, () =>
    withFile(cases[data], async (path) => {
      await assert.rejects(readExact(path, n), UnexpectedEofError);
    }));
}

test("missing file", () =>
  withFile(undefined, async (path) => {
    await assert.rejects(readBytes(path), { code: "ENOENT" });
    await assert.rejects(readExact(path, 1), { code: "ENOENT" });
  }));

test("directory", () =>
  withFile(undefined, async (_, dir) => {
    await assert.rejects(readBytes(dir), { code: "EISDIR" });
    await assert.rejects(readExact(dir, 1), { code: "EISDIR" });
  }));
