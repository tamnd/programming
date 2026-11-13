// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

import assert from "node:assert/strict";
import { existsSync, mkdtempSync, readFileSync, rmSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { CsvError, formatCsv, writeCsv } from "./writeCsv.ts";

const CASES: Record<string, [string[][], string]> = {
  simple: [[["name", "age"], ["Ada", "36"], ["Alan", "41"]], "name,age\r\nAda,36\r\nAlan,41\r\n"],
  comma: [[["name", "born"], ["Lovelace, Ada", "1815"]], 'name,born\r\n"Lovelace, Ada",1815\r\n'],
  "double-quote": [[["id", "quote"], ["1", 'She said "hi".']], 'id,quote\r\n1,"She said ""hi""."\r\n'],
  "only-a-quote": [[["q"], ['"']], 'q\r\n""""\r\n'],
  "header-only": [[["name", "age"]], "name,age\r\n"],
  "newline-in-field": [[["id", "address"], ["1", "12 High St\nLeeds"]], 'id,address\r\n1,"12 High St\nLeeds"\r\n'],
  "crlf-in-field": [[["id", "address"], ["1", "12 High St\r\nLeeds"]], 'id,address\r\n1,"12 High St\r\nLeeds"\r\n'],
  "cr-in-field": [[["id", "text"], ["1", "a\rb"]], 'id,text\r\n1,"a\rb"\r\n'],
  "empty-fields": [[["a", "b", "c"], ["", "", ""]], "a,b,c\r\n,,\r\n"],
  "one-empty-field": [[["note"], [""], ["x"]], 'note\r\n""\r\nx\r\n'],
  spaces: [[["a", "b"], [" x ", " y"]], "a,b\r\n x , y\r\n"],
  "no-records": [[], ""],
  "non-ascii": [[["city"], ["Zürich"], ["東京"]], "city\r\nZürich\r\n東京\r\n"],
};

const ERRORS: Record<string, string[][]> = {
  "too-many-fields": [["a", "b"], ["1", "2", "3"]],
  "too-few-fields": [["a", "b"], ["1"]],
};

const dir = mkdtempSync(join(tmpdir(), "write-csv-"));
after(() => rmSync(dir, { recursive: true, force: true }));

for (const [name, [records, want]] of Object.entries(CASES)) {
  test(name, () => {
    assert.equal(formatCsv(records), want);
    const path = join(dir, `${name}.csv`);
    writeCsv(path, records);
    assert.deepEqual(readFileSync(path), Buffer.from(want, "utf8"));
  });
}

for (const [name, records] of Object.entries(ERRORS)) {
  test(name, () => {
    assert.throws(() => formatCsv(records), (e: unknown) => e instanceof CsvError && e.kind === "field-count");
    const path = join(dir, `${name}.csv`);
    assert.throws(() => writeCsv(path, records), CsvError);
    assert.equal(existsSync(path), false);
  });
}
