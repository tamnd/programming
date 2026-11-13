// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/validate-data-against-a-schema/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { validate } from "./validate.ts";

// [case, schema, instance, the failed [instance location, keyword location] pairs]
const cases: [string, string, string, [string, string][]][] = [
  ["valid-object", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"Ada\", \"age\": 36}", []],
  ["missing-required", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"Ada\"}", [["", "/required"]]],
  ["wrong-member-type", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"Ada\", \"age\": \"36\"}", [["/age", "/properties/age/type"]]],
  ["below-minimum", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"Ada\", \"age\": -1}", [["/age", "/properties/age/minimum"]]],
  ["two-errors", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"\", \"age\": 3.5}", [["/age", "/properties/age/type"], ["/name", "/properties/name/minLength"]]],
  ["array-items", "{\"type\": \"array\", \"items\": {\"type\": \"string\"}}", "[\"a\", 1, \"c\", null]", [["/1", "/items/type"], ["/3", "/items/type"]]],
  ["no-additional", "{\"properties\": {\"id\": {\"type\": \"integer\"}}, \"additionalProperties\": false}", "{\"id\": 1, \"admin\": true}", [["/admin", "/additionalProperties"]]],
  ["integer-from-fraction-zero", "{\"type\": \"integer\"}", "1.0", []],
  ["integer-from-exponent", "{\"type\": \"integer\"}", "1e2", []],
  ["integer-with-fraction", "{\"type\": \"integer\"}", "3.5", [["", "/type"]]],
  ["boolean-not-integer", "{\"type\": \"integer\"}", "true", [["", "/type"]]],
  ["boolean-not-number", "{\"type\": \"number\"}", "false", [["", "/type"]]],
  ["integer-is-number", "{\"type\": \"number\"}", "3", []],
  ["null-not-object", "{\"type\": \"object\"}", "null", [["", "/type"]]],
  ["array-not-object", "{\"type\": \"object\"}", "[]", [["", "/type"]]],
  ["type-list-match", "{\"type\": [\"string\", \"null\"]}", "null", []],
  ["type-list-miss", "{\"type\": [\"string\", \"null\"]}", "5", [["", "/type"]]],
  ["max-length-astral", "{\"maxLength\": 1}", "\"😀\"", []],
  ["min-length-astral", "{\"minLength\": 2}", "\"\\ud83d\\ude00\"", [["", "/minLength"]]],
  ["combining-mark", "{\"maxLength\": 1}", "\"e\\u0301\"", [["", "/maxLength"]]],
  ["other-type-keywords", "{\"minimum\": 5, \"required\": [\"a\"], \"items\": false}", "\"abc\"", []],
  ["other-type-keywords-number", "{\"maxLength\": 1, \"required\": [\"a\"], \"additionalProperties\": false}", "3", []],
  ["unknown-keyword", "{\"minimun\": 5}", "1", []],
  ["format-is-annotation", "{\"type\": \"string\", \"format\": \"email\"}", "\"not an email\"", []],
  ["true-schema", "true", "{\"a\": [1]}", []],
  ["false-schema", "false", "{}", [["", ""]]],
  ["empty-schema", "{}", "null", []],
  ["false-member-schema", "{\"properties\": {\"a\": false}}", "{\"a\": 0, \"b\": 0}", [["/a", "/properties/a"]]],
  ["pointer-escape", "{\"properties\": {\"a/b\": {\"type\": \"string\"}, \"m~n\": {\"type\": \"string\"}}}", "{\"a/b\": 1, \"m~n\": 2}", [["/a~1b", "/properties/a~1b/type"], ["/m~0n", "/properties/m~0n/type"]]],
  ["pointer-escape-order", "{\"properties\": {\"~1\": false}}", "{\"~1\": 0}", [["/~01", "/properties/~01"]]],
  ["nested-items", "{\"items\": {\"items\": {\"type\": \"integer\"}}}", "[[1, \"x\"], [2]]", [["/0/1", "/items/items/type"]]],
  ["additional-schema", "{\"properties\": {\"id\": {}}, \"additionalProperties\": {\"type\": \"string\"}}", "{\"id\": 1, \"a\": \"x\", \"b\": 2}", [["/b", "/additionalProperties/type"]]],
  ["absent-property", "{\"properties\": {\"a\": {\"type\": \"string\"}}}", "{}", []],
  ["required-all-missing", "{\"required\": [\"a\", \"b\", \"c\"]}", "{}", [["", "/required"]]],
  ["required-null-member", "{\"required\": [\"a\"]}", "{\"a\": null}", []],
  ["inclusive-bounds", "{\"items\": {\"minimum\": 0, \"maximum\": 10}}", "[0, 10, 10.5, -0.5]", [["/2", "/items/maximum"], ["/3", "/items/minimum"]]],
  ["negative-zero", "{\"minimum\": 0}", "-0", []],
  ["fraction-minimum", "{\"minimum\": 0.5}", "0.4", [["", "/minimum"]]],
  ["index-order", "{\"items\": {\"type\": \"string\"}}", "[0, \"\", 0, \"\", \"\", \"\", \"\", \"\", \"\", \"\", 0]", [["/0", "/items/type"], ["/10", "/items/type"], ["/2", "/items/type"]]],
  ["keyword-order", "{\"type\": \"string\", \"minimum\": 5}", "3", [["", "/minimum"], ["", "/type"]]],
  ["inherited-name", "{\"required\": [\"toString\", \"constructor\"]}", "{}", [["", "/required"]]],
  ["proto-member", "{\"properties\": {\"__proto__\": {\"type\": \"string\"}}}", "{\"__proto__\": 1}", [["/__proto__", "/properties/__proto__/type"]]],
  ["non-ascii-name", "{\"required\": [\"é\"], \"properties\": {\"日本\": {\"type\": \"integer\"}}}", "{\"é\": 1, \"日本\": \"x\"}", [["/日本", "/properties/日本/type"]]],
  ["deep-path", "{\"properties\": {\"a\": {\"items\": {\"properties\": {\"b\": {\"required\": [\"c\"]}}}}}}", "{\"a\": [{\"b\": {}}, {\"b\": {\"c\": 1}}, {\"b\": {}}]}", [["/a/0/b", "/properties/a/items/properties/b/required"], ["/a/2/b", "/properties/a/items/properties/b/required"]]],
];

// [case, schema, instance]; one of the texts is not JSON
const invalid: [string, string, string][] = [
  ["trailing-comma", "{\"type\": \"array\"}", "[1, 2,]"],
  ["nan", "{\"type\": \"number\"}", "NaN"],
  ["empty-instance", "{}", ""],
  ["bad-schema", "{\"type\": ", "1"],
  ["single-quotes", "{}", "{'a': 1}"],
];

for (const [name, schema, instance, want] of cases) {
  test(name, () => assert.deepEqual(validate(schema, instance), want));
}

for (const [name, schema, instance] of invalid) {
  test(name, () => assert.throws(() => validate(schema, instance), SyntaxError));
}
