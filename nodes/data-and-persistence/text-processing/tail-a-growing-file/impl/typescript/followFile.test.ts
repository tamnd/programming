// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

import assert from "node:assert/strict";
import { appendFileSync, mkdtempSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { fileURLToPath } from "node:url";
import { test } from "node:test";
import { Follower, followFile } from "./followFile.ts";

interface Case {
  name: string;
  args: { path: string; initial: string; steps: string[] };
  expect: string[][];
}

const node = fileURLToPath(new URL("../../", import.meta.url));

function load(name: string): Case[] {
  return readFileSync(join(node, "cases", name), "utf8")
    .split("\n")
    .filter((line) => line.trim() !== "")
    .map((line) => JSON.parse(line) as Case);
}

function inTempDir(fn: (dir: string) => void): void {
  const dir = mkdtempSync(join(tmpdir(), "follow-file-"));
  try {
    fn(dir);
  } finally {
    rmSync(dir, { recursive: true, force: true });
  }
}

for (const c of [...load("basic.jsonl"), ...load("edge.jsonl")]) {
  test(c.name, () => {
    inTempDir((dir) => {
      assert.deepEqual(followFile(join(dir, c.args.path), c.args.initial, c.args.steps), c.expect);
    });
  });
}

test("not-found", () => {
  inTempDir((dir) => {
    assert.throws(() => followFile(join(dir, "missing", "followed.txt"), "", ["poll"]), { code: "ENOENT" });
  });
});

test("many-lines", () => {
  inTempDir((dir) => {
    const path = join(dir, "followed.txt");
    writeFileSync(path, "");
    const follower = new Follower(path);
    let text = "";
    for (let i = 0; i < 100000; i++) {
      text += `${i}\n`;
    }
    appendFileSync(path, text);
    const lines = follower.poll();
    follower.close();
    assert.equal(lines.length, 100000);
    assert.equal(lines[0], "0");
    assert.equal(lines.at(-1), "99999");
  });
});

test("character-across-polls", () => {
  inTempDir((dir) => {
    const path = join(dir, "followed.txt");
    writeFileSync(path, "");
    const follower = new Follower(path);
    // The two bytes of U+00E9 come in two polls.
    appendFileSync(path, Buffer.from([0x63, 0x61, 0x66, 0xc3]));
    assert.deepEqual(follower.poll(), []);
    appendFileSync(path, Buffer.from([0xa9, 0x0a]));
    assert.deepEqual(follower.poll(), ["caf\u00e9"]);
    follower.close();
  });
});
