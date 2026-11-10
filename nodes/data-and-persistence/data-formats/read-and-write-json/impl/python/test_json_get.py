# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-and-write-json/.

import json

import pytest

from json_get import json_get

# name: (text, path, the canonical JSON of the value at path)
CASES = {
    "member": ('{"name": "Ada", "born": 1815}', ["name"], '"Ada"'),
    "whole-document": ('{"b": [1, 2], "a": true}', [], '{"a":true,"b":[1,2]}'),
    "nested-path": ('{"user": {"tags": ["x", "y"]}}', ["user", "tags", "1"], '"y"'),
    "big-int": ('{"id": 9007199254740993}', ["id"], "9007199254740993"),
    "beyond-int64": (
        "[18446744073709551617, -18446744073709551617]",
        [],
        "[18446744073709551617,-18446744073709551617]",
    ),
    "int64-bounds": ("[-9223372036854775808, 9223372036854775807]", [], "[-9223372036854775808,9223372036854775807]"),
    "negative-zero": ("[-0, 0]", [], "[0,0]"),
    "duplicate-name": ('{"a": 1, "a": 2}', ["a"], "2"),
    "duplicate-name-whole": ('{"a": 1, "b": 0, "a": {"c": 3}}', [], '{"a":{"c":3},"b":0}'),
    "array-index-10": ('[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "ten"]', ["10"], '"ten"'),
    "numeric-name": ('{"0": "zero", "1": "one"}', ["0"], '"zero"'),
    "key-order-ascii": ('{"b": 1, "a": 2, "B": 3, "10": 4, "9": 5}', [], '{"10":4,"9":5,"B":3,"a":2,"b":1}'),
    "key-order-prefix": ('{"ab": 1, "a": 2, "": 3}', [], '{"":3,"a":2,"ab":1}'),
    "key-order-astral": ('{"😀": 1, "ﬁ": 2, "é": 3}', [], '{"é":3,"ﬁ":2,"😀":1}'),
    "non-ascii": ('["Zürich", "日本語", "😀"]', [], '["Zürich","日本語","😀"]'),
    "escaped-non-ascii": ('["\\u00e9\\ud83d\\ude00"]', ["0"], '"é😀"'),
    "escapes": ('["q\\" b\\\\ s\\/ t\\t n\\n r\\r"]', [], '["q\\" b\\\\ s/ t\\t n\\n r\\r"]'),
    "control-chars": ('["\\u0000\\u001f\\b\\f\\u007f"]', [], '["\\u0000\\u001f\\b\\f\x7f"]'),
    "html-chars": ('{"html": "<a href=\'x\'>&amp;</a>"}', ["html"], "\"<a href='x'>&amp;</a>\""),
    "empty-containers": ('{"a": {}, "b": [], "c": ""}', [], '{"a":{},"b":[],"c":""}'),
    "scalar-root": ("42", [], "42"),
    "string-root": ('"text"', [], '"text"'),
    "whitespace": (" \n\ttrue\r\n ", [], "true"),
    "whitespace-inside": ('\n{ "a" :\t[ 1 , 2 ] }\r\n', [], '{"a":[1,2]}'),
    "deep-path": ('{"a": [{"b": [[1, {"c": "deep"}]]}]}', ["a", "0", "b", "0", "1", "c"], '"deep"'),
    "null-member": ('{"a": null}', ["a"], "null"),
    "false-member": ('{"a": false, "b": 0}', ["a"], "false"),
    "proto-name": ('{"__proto__": {"x": 1}, "constructor": 2}', [], '{"__proto__":{"x":1},"constructor":2}'),
    "proto-path": ('{"__proto__": {"x": 1}}', ["__proto__", "x"], "1"),
}

# name: (text, path); the text is not JSON
INVALID = {
    "trailing-comma-array": ("[1, 2,]", []),
    "trailing-comma-object": ('{"a": 1,}', []),
    "single-quotes": ("{'a': 1}", []),
    "unquoted-name": ("{a: 1}", []),
    "nan": ("[NaN]", []),
    "infinity": ("[Infinity]", []),
    "negative-infinity": ("-Infinity", []),
    "leading-zero": ("[01]", []),
    "plus-sign": ("+1", []),
    "hex-number": ("0x10", []),
    "empty-text": ("", []),
    "whitespace-only": (" \n", []),
    "trailing-data": ("[1] x", []),
    "two-values": ("1 2", []),
    "comment": ('{"a": 1} // note', []),
    "byte-order-mark": ('\ufeff{"a": 1}', []),
    "raw-control-char": ('["a\tb"]', []),
    "bad-escape": ('["\\x41"]', []),
    "unclosed": ("[1, 2", []),
}

# name: (text, path); the path names no value
NOT_FOUND = {
    "missing-name": ('{"a": 1}', ["b"]),
    "inherited-name": ('{"a": 1}', ["toString"]),
    "array-length": ("[1, 2]", ["length"]),
    "index-out-of-range": ("[1, 2]", ["2"]),
    "index-leading-zero": ("[1, 2]", ["01"]),
    "index-negative": ("[1, 2]", ["-1"]),
    "index-plus": ("[1, 2]", ["+1"]),
    "index-non-ascii-digit": ("[1, 2]", ["١"]),
    "through-scalar": ('{"a": 1}', ["a", "b"]),
    "through-null": ('{"a": null}', ["a", "b"]),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(name: str) -> None:
    text, path, want = CASES[name]
    assert json_get(text, path) == want


@pytest.mark.parametrize("name", INVALID)
def test_invalid_json(name: str) -> None:
    text, path = INVALID[name]
    with pytest.raises(ValueError):
        json_get(text, path)


@pytest.mark.parametrize("name", NOT_FOUND)
def test_not_found(name: str) -> None:
    text, path = NOT_FOUND[name]
    with pytest.raises(KeyError):
        json_get(text, path)


def test_decode_error_type() -> None:
    # JSONDecodeError is a subclass of ValueError. It gives the position of the error: here the comma.
    with pytest.raises(json.JSONDecodeError) as info:
        json_get("[1, 2,]", [])
    assert (info.value.lineno, info.value.colno) == (1, 6)
