// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/stream-parse-a-large-json-file/.

import assert from "node:assert/strict";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, test } from "node:test";

import { BadRecordError, streamSum } from "./streamSum.js";

// 2500 records. The second space in the first record puts the digits of one amount across the offset 65536, so they come in two chunks.
function manyRecords() {
  const records = Array.from({ length: 2500 }, (_, i) => `{"id": ${i},${i === 0 ? "  " : " "}"amount": ${((i * 37) % 1000) - 400}}`);
  return "[" + records.join(",\n") + "]\n";
}

const CASES = {
  "three-records": ['[{"id": 1, "amount": 10}, {"id": 2, "amount": 25}, {"id": 3, "amount": -5}]\n', [3, 30]],
  "pretty": ['[\n  {\n    "id": 1,\n    "amount": 100\n  },\n  {\n    "id": 2,\n    "amount": 250\n  }\n]\n', [2, 350]],
  "empty-array": ['[]\n', [0, 0]],
  "one-record": ['[{"amount": 7}]', [1, 7]],
  "extra-members": ['[{"id": 1, "tags": ["a", "b"], "meta": {"amount": 999, "x": [1, [2]]}, "amount": 5}, {"amount": 6, "ok": true, "none": null}]\n', [2, 11]],
  "strings-with-brackets": ['[{"note": "a ] b , c } \\" [ {", "amount": 1}, {"note": "\\\\", "amount": 2}, {"note": "]", "amount": 3}]\n', [3, 6]],
  "non-ascii": ['[{"name": "Zürich 😀", "amount": 3}, {"name": "日本語", "amount": 4}]\n', [2, 7]],
  "crlf": ['[\r\n{"amount": 1},\r\n{"amount": 2}\r\n]\r\n', [2, 3]],
  "bom": ['\uFEFF[{"amount": 4}]\n', [1, 4]],
  "big-total": ['[{"amount": 4503599627370496}, {"amount": 4503599627370495}, {"amount": 0}]\n', [3, 9007199254740991]],
  "negative-total": ['[{"amount": -9007199254740991}, {"amount": 0}]\n', [2, -9007199254740991]],
  "whitespace": [' \n\t[ {"amount" : 1} , {"amount":2} ]  \n\n', [2, 3]],
  // A string of 70000 characters is longer than one chunk.
  "long-string": ['[{"note": "' + "x".repeat(70000) + '", "amount": 8}, {"amount": 9}]\n', [2, 17]],
  "many-records": [manyRecords(), [2500, 249750]],
};

const INVALID = {
  "not-array": '{"amount": 1}\n',
  "trailing-comma": '[{"amount": 1},]\n',
  "missing-comma": '[{"amount": 1} {"amount": 2}]\n',
  "unclosed": '[{"amount": 1},\n',
  "unclosed-element": '[{"amount": 1}, {"amount": 2\n',
  "data-after": '[{"amount": 1}] x\n',
  "two-arrays": '[] []\n',
  "empty-file": '',
  "bad-element": '[{"amount": 1}, {"amount": }]\n',
  "nan": '[{"amount": NaN}]\n',
};

const BAD_RECORDS = {
  "not-object": '[1, 2]\n',
  "missing-amount": '[{"amount": 1}, {"id": 2}]\n',
  "string-amount": '[{"amount": "5"}]\n',
  "null-amount": '[{"amount": null}]\n',
  "bool-amount": '[{"amount": true}]\n',
  "nested-array": '[[{"amount": 1}]]\n',
};

const dir = mkdtempSync(join(tmpdir(), "stream-sum-"));
after(() => rmSync(dir, { recursive: true, force: true }));

function write(name, text) {
  const path = join(dir, `${name}.json`);
  writeFileSync(path, text, "utf8");
  return path;
}

for (const [name, [text, want]] of Object.entries(CASES)) {
  test(name, () => {
    assert.deepEqual(streamSum(write(name, text)), want);
  });
}

for (const [name, text] of Object.entries(INVALID)) {
  test(name, () => {
    assert.throws(() => streamSum(write(name, text)), SyntaxError);
  });
}

for (const [name, text] of Object.entries(BAD_RECORDS)) {
  test(name, () => {
    assert.throws(() => streamSum(write(name, text)), BadRecordError);
  });
}

test("missing", () => {
  assert.throws(() => streamSum(join(dir, "missing.json")), { code: "ENOENT" });
});
