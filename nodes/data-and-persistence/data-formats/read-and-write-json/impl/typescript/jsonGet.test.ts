// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-json/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { jsonGet } from "./jsonGet.ts";

// [case, text, path, the canonical JSON of the value at path]
const cases: [string, string, string[], string][] = [
  ["member", '{"name": "Ada", "born": 1815}', ["name"], '"Ada"'],
  ["whole-document", '{"b": [1, 2], "a": true}', [], '{"a":true,"b":[1,2]}'],
  ["nested-path", '{"user": {"tags": ["x", "y"]}}', ["user", "tags", "1"], '"y"'],
  ["big-int", '{"id": 9007199254740993}', ["id"], "9007199254740993"],
  ["beyond-int64", "[18446744073709551617, -18446744073709551617]", [], "[18446744073709551617,-18446744073709551617]"],
  ["int64-bounds", "[-9223372036854775808, 9223372036854775807]", [], "[-9223372036854775808,9223372036854775807]"],
  ["negative-zero", "[-0, 0]", [], "[0,0]"],
  ["duplicate-name", '{"a": 1, "a": 2}', ["a"], "2"],
  ["duplicate-name-whole", '{"a": 1, "b": 0, "a": {"c": 3}}', [], '{"a":{"c":3},"b":0}'],
  ["array-index-10", '[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "ten"]', ["10"], '"ten"'],
  ["numeric-name", '{"0": "zero", "1": "one"}', ["0"], '"zero"'],
  ["key-order-ascii", '{"b": 1, "a": 2, "B": 3, "10": 4, "9": 5}', [], '{"10":4,"9":5,"B":3,"a":2,"b":1}'],
  ["key-order-prefix", '{"ab": 1, "a": 2, "": 3}', [], '{"":3,"a":2,"ab":1}'],
  ["key-order-astral", '{"😀": 1, "ﬁ": 2, "é": 3}', [], '{"é":3,"ﬁ":2,"😀":1}'],
  ["non-ascii", '["Zürich", "日本語", "😀"]', [], '["Zürich","日本語","😀"]'],
  ["escaped-non-ascii", '["\\u00e9\\ud83d\\ude00"]', ["0"], '"é😀"'],
  ["escapes", '["q\\" b\\\\ s\\/ t\\t n\\n r\\r"]', [], '["q\\" b\\\\ s/ t\\t n\\n r\\r"]'],
  ["control-chars", '["\\u0000\\u001f\\b\\f\\u007f"]', [], '["\\u0000\\u001f\\b\\f\x7f"]'],
  ["html-chars", '{"html": "<a href=\'x\'>&amp;</a>"}', ["html"], "\"<a href='x'>&amp;</a>\""],
  ["empty-containers", '{"a": {}, "b": [], "c": ""}', [], '{"a":{},"b":[],"c":""}'],
  ["scalar-root", "42", [], "42"],
  ["string-root", '"text"', [], '"text"'],
  ["whitespace", " \n\ttrue\r\n ", [], "true"],
  ["whitespace-inside", '\n{ "a" :\t[ 1 , 2 ] }\r\n', [], '{"a":[1,2]}'],
  ["deep-path", '{"a": [{"b": [[1, {"c": "deep"}]]}]}', ["a", "0", "b", "0", "1", "c"], '"deep"'],
  ["null-member", '{"a": null}', ["a"], "null"],
  ["false-member", '{"a": false, "b": 0}', ["a"], "false"],
  ["proto-name", '{"__proto__": {"x": 1}, "constructor": 2}', [], '{"__proto__":{"x":1},"constructor":2}'],
  ["proto-path", '{"__proto__": {"x": 1}}', ["__proto__", "x"], "1"],
];

// [case, text, path]; the text is not JSON
const invalid: [string, string, string[]][] = [
  ["trailing-comma-array", "[1, 2,]", []],
  ["trailing-comma-object", '{"a": 1,}', []],
  ["single-quotes", "{'a': 1}", []],
  ["unquoted-name", "{a: 1}", []],
  ["nan", "[NaN]", []],
  ["infinity", "[Infinity]", []],
  ["negative-infinity", "-Infinity", []],
  ["leading-zero", "[01]", []],
  ["plus-sign", "+1", []],
  ["hex-number", "0x10", []],
  ["empty-text", "", []],
  ["whitespace-only", " \n", []],
  ["trailing-data", "[1] x", []],
  ["two-values", "1 2", []],
  ["comment", '{"a": 1} // note', []],
  ["byte-order-mark", '\ufeff{"a": 1}', []],
  ["raw-control-char", '["a\tb"]', []],
  ["bad-escape", '["\\x41"]', []],
  ["unclosed", "[1, 2", []],
];

// [case, text, path]; the path names no value
const notFound: [string, string, string[]][] = [
  ["missing-name", '{"a": 1}', ["b"]],
  ["inherited-name", '{"a": 1}', ["toString"]],
  ["array-length", "[1, 2]", ["length"]],
  ["index-out-of-range", "[1, 2]", ["2"]],
  ["index-leading-zero", "[1, 2]", ["01"]],
  ["index-negative", "[1, 2]", ["-1"]],
  ["index-plus", "[1, 2]", ["+1"]],
  ["index-non-ascii-digit", "[1, 2]", ["١"]],
  ["through-scalar", '{"a": 1}', ["a", "b"]],
  ["through-null", '{"a": null}', ["a", "b"]],
];

for (const [name, text, path, want] of cases) {
  test(name, () => {
    assert.equal(jsonGet(text, path), want);
  });
}

for (const [name, text, path] of invalid) {
  test(name, () => {
    assert.throws(() => jsonGet(text, path), SyntaxError);
  });
}

for (const [name, text, path] of notFound) {
  test(name, () => {
    assert.throws(() => jsonGet(text, path), RangeError);
  });
}
