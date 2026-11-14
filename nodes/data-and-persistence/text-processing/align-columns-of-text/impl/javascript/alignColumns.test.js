// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import { join } from "node:path";
import { fileURLToPath } from "node:url";
import { test } from "node:test";
import { alignColumns } from "./alignColumns.js";

const node = fileURLToPath(new URL("../../", import.meta.url));

function load(name) {
  return readFileSync(join(node, "cases", name), "utf8")
    .split("\n")
    .filter((line) => line.trim() !== "")
    .map((line) => JSON.parse(line));
}

for (const c of [...load("basic.jsonl"), ...load("edge.jsonl")]) {
  test(c.name, () => {
    assert.equal(alignColumns(c.args.text, c.args.align), c.expect);
  });
}

test("many-lines", () => {
  let text = "";
  for (let i = 0; i < 100000; i++) {
    text += `${i}$x\n`;
  }
  const lines = alignColumns(text, "right").split("\n");
  assert.equal(lines.length, 100001);
  assert.equal(lines[0], "    0 x");
  assert.equal(lines[99999], "99999 x");
});

for (const c of load("errors.jsonl")) {
  test(c.name, () => {
    assert.throws(() => alignColumns(c.args.text, c.args.align), RangeError);
  });
}
