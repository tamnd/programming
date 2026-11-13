// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-csv-to-json/.

import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import { test } from "node:test";

import { CsvError, csvToJson } from "./csvToJson.ts";

const load = (name: string): { name: string; args: { text: string }; expect?: string; error?: string }[] =>
  readFileSync(new URL(`../../cases/${name}`, import.meta.url), "utf8")
    .split("\n")
    .filter((line) => line.trim() !== "")
    .map((line) => JSON.parse(line));

for (const c of [...load("basic.jsonl"), ...load("edge.jsonl")]) {
  test(c.name, () => {
    assert.equal(csvToJson(c.args.text), c.expect);
  });
}

for (const c of load("errors.jsonl")) {
  test(c.name, () => {
    assert.throws(() => csvToJson(c.args.text), (e: unknown) => e instanceof CsvError && e.kind === c.error);
  });
}

test("the output is JSON", () => {
  const text = 'id,note\n1,"She said ""hi""\nand left"\n';
  assert.deepEqual(JSON.parse(csvToJson(text)), [{ id: "1", note: 'She said "hi"\nand left' }]);
});
