// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

import assert from "node:assert/strict";
import { chmodSync, cpSync, existsSync, mkdirSync, mkdtempSync, readdirSync, readFileSync, rmSync, statSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { basename, join } from "node:path";
import { fileURLToPath } from "node:url";
import { after, test } from "node:test";
import { replaceInFiles } from "./replaceInFiles.js";

const node = fileURLToPath(new URL("../../", import.meta.url));
const root = mkdtempSync(join(tmpdir(), "replace-in-files-"));
after(() => rmSync(root, { recursive: true }));

function load(name) {
  return readFileSync(join(node, "cases", name), "utf8")
    .split("\n")
    .filter((line) => line.trim() !== "")
    .map((line) => JSON.parse(line));
}

// Copies fixtures/NAME to a new directory and returns the directory.
function copyCase(name) {
  const dir = join(mkdtempSync(join(root, "case-")), name);
  const src = join(node, "fixtures", name);
  if (existsSync(src)) cpSync(src, dir, { recursive: true });
  else mkdirSync(dir);
  return dir;
}

for (const c of [...load("basic.jsonl"), ...load("edge.jsonl")]) {
  test(c.name, () => {
    const dir = copyCase(c.name);
    const paths = c.args.paths.map((p) => join(dir, basename(p)));
    assert.deepEqual(replaceInFiles(paths, c.args.old, c.args.new), c.expect);
    for (const p of paths) {
      assert.deepEqual(readFileSync(p), readFileSync(join(node, "fixtures", c.name + ".after", basename(p))));
    }
    // No temporary file stays in the directory.
    assert.deepEqual(readdirSync(dir).sort(), paths.map((p) => basename(p)).sort());
  });
}

test("many-lines", () => {
  const path = join(mkdtempSync(join(root, "case-")), "many.txt");
  const lines = (prefix) => Array.from({ length: 100000 }, (_, i) => `${prefix}${i}\n`).join("");
  writeFileSync(path, lines("line "));
  assert.deepEqual(replaceInFiles([path], "line ", "L"), [100000]);
  assert.equal(readFileSync(path, "utf8"), lines("L"));
});

test("keeps-the-permissions", { skip: process.platform === "win32" }, () => {
  const path = join(mkdtempSync(join(root, "case-")), "a.txt");
  writeFileSync(path, "a\n");
  chmodSync(path, 0o640);
  assert.deepEqual(replaceInFiles([path], "a", "b"), [1]);
  assert.equal(statSync(path).mode & 0o777, 0o640);
});

test("errors", () => {
  assert.deepEqual(load("errors.jsonl").map((c) => c.name), ["not-found", "empty-old", "stop"]);
  assert.throws(() => replaceInFiles([join(root, "missing.txt")], "a", "b"), { code: "ENOENT" });
  const path = join(mkdtempSync(join(root, "case-")), "a.txt");
  writeFileSync(path, "a\n");
  assert.throws(() => replaceInFiles([path], "", "b"), RangeError);
  assert.equal(readFileSync(path, "utf8"), "a\n");
});

test("stop", () => {
  const dir = copyCase("stop");
  const names = ["a.txt", "missing.txt", "c.txt"];
  assert.throws(() => replaceInFiles(names.map((n) => join(dir, n)), "a", "b"), { code: "ENOENT" });
  for (const n of ["a.txt", "c.txt"]) {
    assert.deepEqual(readFileSync(join(dir, n)), readFileSync(join(node, "fixtures", "stop.after", n)));
  }
});
