# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/stream-parse-a-large-json-file/.

from pathlib import Path

import pytest

from stream_sum import BadRecord, stream_sum


def many_records() -> str:
    # 2500 records. The second space in the first record puts the digits of one amount across the offset 65536, so they come in two chunks.
    records = [f'{{"id": {i},{"  " if i == 0 else " "}"amount": {i * 37 % 1000 - 400}}}' for i in range(2500)]
    return "[" + ",\n".join(records) + "]\n"


CASES = {
    "three-records": ('[{"id": 1, "amount": 10}, {"id": 2, "amount": 25}, {"id": 3, "amount": -5}]\n', [3, 30]),
    "pretty": ('[\n  {\n    "id": 1,\n    "amount": 100\n  },\n  {\n    "id": 2,\n    "amount": 250\n  }\n]\n', [2, 350]),
    "empty-array": ('[]\n', [0, 0]),
    "one-record": ('[{"amount": 7}]', [1, 7]),
    "extra-members": ('[{"id": 1, "tags": ["a", "b"], "meta": {"amount": 999, "x": [1, [2]]}, "amount": 5}, {"amount": 6, "ok": true, "none": null}]\n', [2, 11]),
    "strings-with-brackets": ('[{"note": "a ] b , c } \\" [ {", "amount": 1}, {"note": "\\\\", "amount": 2}, {"note": "]", "amount": 3}]\n', [3, 6]),
    "non-ascii": ('[{"name": "Zürich 😀", "amount": 3}, {"name": "日本語", "amount": 4}]\n', [2, 7]),
    "crlf": ('[\r\n{"amount": 1},\r\n{"amount": 2}\r\n]\r\n', [2, 3]),
    "bom": ('\uFEFF[{"amount": 4}]\n', [1, 4]),
    "big-total": ('[{"amount": 4503599627370496}, {"amount": 4503599627370495}, {"amount": 0}]\n', [3, 9007199254740991]),
    "negative-total": ('[{"amount": -9007199254740991}, {"amount": 0}]\n', [2, -9007199254740991]),
    "whitespace": (' \n\t[ {"amount" : 1} , {"amount":2} ]  \n\n', [2, 3]),
    # A string of 70000 characters is longer than one chunk.
    "long-string": ('[{"note": "' + "x" * 70000 + '", "amount": 8}, {"amount": 9}]\n', [2, 17]),
    "many-records": (many_records(), [2500, 249750]),
}

INVALID = {
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
}

BAD_RECORDS = {
    "not-object": '[1, 2]\n',
    "missing-amount": '[{"amount": 1}, {"id": 2}]\n',
    "string-amount": '[{"amount": "5"}]\n',
    "null-amount": '[{"amount": null}]\n',
    "bool-amount": '[{"amount": true}]\n',
    "nested-array": '[[{"amount": 1}]]\n',
}


def write(tmp_path: Path, text: str) -> str:
    path = tmp_path / "input.json"
    path.write_bytes(text.encode())
    return str(path)


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    text, want = CASES[name]
    assert stream_sum(write(tmp_path, text)) == want


@pytest.mark.parametrize("name", INVALID)
def test_invalid(tmp_path: Path, name: str) -> None:
    with pytest.raises(ValueError):
        stream_sum(write(tmp_path, INVALID[name]))


@pytest.mark.parametrize("name", BAD_RECORDS)
def test_bad_record(tmp_path: Path, name: str) -> None:
    with pytest.raises(BadRecord):
        stream_sum(write(tmp_path, BAD_RECORDS[name]))


def test_missing(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        stream_sum(str(tmp_path / "missing.json"))
