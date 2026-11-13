// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-a-markdown-text/.

import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import { test } from "node:test";

import { markdownToHtml } from "./markdownToHtml.ts";

const load = (name) =>
  readFileSync(new URL(`../../cases/${name}`, import.meta.url), "utf8")
    .split("\n")
    .filter((line) => line.trim() !== "")
    .map((line) => JSON.parse(line));

for (const c of [...load("basic.jsonl"), ...load("edge.jsonl")]) {
  test(c.name, () => {
    assert.equal(markdownToHtml(c.args.text), c.expect);
  });
}
