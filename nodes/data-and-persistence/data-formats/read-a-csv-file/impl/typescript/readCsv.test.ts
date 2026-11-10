// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { CsvError, readCsv } from "./readCsv.ts";

const ADA_ALAN = [
  { name: "Ada", age: "36" },
  { name: "Alan", age: "41" },
];

const CASES: Record<string, [string, Record<string, string>[]]> = {
  simple: ["name,age\nAda,36\nAlan,41\n", ADA_ALAN],
  crlf: ["name,age\r\nAda,36\r\nAlan,41\r\n", ADA_ALAN],
  "no-final-newline": ["name,age\nAda,36\nAlan,41", ADA_ALAN],
  "quoted-comma": ['name,born\n"Lovelace, Ada",1815\n', [{ name: "Lovelace, Ada", born: "1815" }]],
  "escaped-quote": ['id,quote\n1,"She said ""hi""."\n', [{ id: "1", quote: 'She said "hi".' }]],
  "quoted-header": ['"first name","last name"\nAda,Lovelace\n', [{ "first name": "Ada", "last name": "Lovelace" }]],
  "newline-in-field": ['id,address\n1,"12 High St\nLeeds"\n', [{ id: "1", address: "12 High St\nLeeds" }]],
  "crlf-in-field": ['id,address\r\n1,"12 High St\r\nLeeds"\r\n', [{ id: "1", address: "12 High St\nLeeds" }]],
  "blank-line-in-field": ['id,note\n1,"one\n\ntwo"\n', [{ id: "1", note: "one\n\ntwo" }]],
  "empty-last-line": ["a,b\n1,2\n\n", [{ a: "1", b: "2" }]],
  "blank-lines": ["a,b\n\n1,2\n\n\n3,4\n", [{ a: "1", b: "2" }, { a: "3", b: "4" }]],
  "empty-fields": ["a,b,c\n,,\n", [{ a: "", b: "", c: "" }]],
  "quoted-empty": ['name\n""\n', [{ name: "" }]],
  spaces: ["a,b\n x , y \n", [{ a: " x ", b: " y " }]],
  "header-only": ["a,b\n", []],
  "empty-file": ["", []],
  bom: ["\uFEFFname,age\nAda,36\n", [{ name: "Ada", age: "36" }]],
  "non-ascii": ["city,country\nZürich,Schweiz\n東京,日本\n", [{ city: "Zürich", country: "Schweiz" }, { city: "東京", country: "日本" }]],
};

const INVALID: Record<string, [string, string]> = {
  "too-few-fields": ["a,b\n1\n", "field-count"],
  "too-many-fields": ["a,b\n1,2,3\n", "field-count"],
  "unclosed-quote": ['a,b\n1,"two\n', "bad-quote"],
  "text-after-quote": ['a,b\n1,"two"x\n', "bad-quote"],
};

const dir = mkdtempSync(join(tmpdir(), "read-csv-"));
after(() => rmSync(dir, { recursive: true, force: true }));

function write(name: string, text: string): string {
  const path = join(dir, `${name}.csv`);
  writeFileSync(path, text, "utf8");
  return path;
}

for (const [name, [text, want]] of Object.entries(CASES)) {
  test(name, async () => {
    assert.deepEqual(await readCsv(write(name, text)), want);
  });
}

for (const [name, [text, kind]] of Object.entries(INVALID)) {
  test(name, async () => {
    await assert.rejects(readCsv(write(name, text)), (e: unknown) => e instanceof CsvError && e.kind === kind);
  });
}

test("missing", async () => {
  await assert.rejects(readCsv(join(dir, "missing.csv")), { code: "ENOENT" });
});

test("proto header", async () => {
  const [record] = await readCsv(write("proto", "__proto__\nx\n"));
  assert.equal(Object.getOwnPropertyDescriptor(record, "__proto__")?.value, "x");
});
