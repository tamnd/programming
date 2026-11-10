// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

import assert from "node:assert/strict";
import { mkdtemp, rm, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { test } from "node:test";

import { sha256File, sha256Text } from "./sha256Hash.js";

// [case, text, the SHA-256 of its UTF-8 bytes]
const textCases = [
  ["abc", "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"],
  [
    "two-block-56",
    "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1",
  ],
  [
    "fox",
    "The quick brown fox jumps over the lazy dog",
    "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592",
  ],
  ["empty", "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"],
  ["a-55", "a".repeat(55), "9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318"],
  ["a-56", "a".repeat(56), "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a"],
  ["a-63", "a".repeat(63), "7d3e74a05d7db15bce4ad9ec0658ea98e3f06eeecf16b4c6fff2da457ddc2f34"],
  ["a-64", "a".repeat(64), "ffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb"],
  [
    "multi-block-112",
    "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn" +
      "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
    "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1",
  ],
  [
    "non-ascii",
    "naïve café 日本語 \u{1f642}",
    "9c53e090acd67abd65c64ee431a36bdc36c3fd86341f5814a94d1b901ca9ed8d",
  ],
  ["nfc-cafe", "caf\u00e9", "850f7dc43910ff890f8879c0ed26fe697c93a067ad93a7d50f466a7028a9bf4e"],
  ["nfd-cafe", "cafe\u0301", "81ef060bcd98adc7824eb5c1ada83c32491b16018e11e79f00ab9d09e04b015a"],
  ["trailing-newline", "abc\n", "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"],
];

// [case, the bytes of the file, its SHA-256], from cases/files.jsonl
const fileCases = [
  ["abc-file", Buffer.from("abc"), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"],
  ["abc-newline-file", Buffer.from("abc\n"), "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"],
  ["abc-crlf-file", Buffer.from("abc\r\n"), "552bab6864c7a7b69a502ed1854b9245c0e1a30f008aaa0b281da62585fdb025"],
  ["empty-file", Buffer.alloc(0), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"],
  ["binary-file", Buffer.from([0x00, 0x01, 0xff]), "26a66b061e8f48f39927c312f25293959729eee95978e2892d49d3512a5cc092"],
  // One million repetitions of "a": more than one chunk.
  ["million-a-file", Buffer.alloc(1_000_000, "a"), "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"],
];

for (const [name, text, want] of textCases) {
  test(name, () => {
    assert.equal(sha256Text(text), want);
  });
}

for (const [name, data, want] of fileCases) {
  test(name, async () => {
    const dir = await mkdtemp(join(tmpdir(), "sha256-"));
    try {
      const path = join(dir, "in.bin");
      await writeFile(path, data);
      assert.equal(await sha256File(path), want);
    } finally {
      await rm(dir, { recursive: true });
    }
  });
}

test("missing-file", async () => {
  const dir = await mkdtemp(join(tmpdir(), "sha256-"));
  try {
    await assert.rejects(sha256File(join(dir, "missing.txt")), { code: "ENOENT" });
  } finally {
    await rm(dir, { recursive: true });
  }
});
