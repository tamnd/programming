# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-csv-file/.

import csv
from pathlib import Path

import pytest

from read_csv import read_csv

ADA_ALAN = [{"name": "Ada", "age": "36"}, {"name": "Alan", "age": "41"}]

CASES = {
    "simple": (b"name,age\nAda,36\nAlan,41\n", ADA_ALAN),
    "crlf": (b"name,age\r\nAda,36\r\nAlan,41\r\n", ADA_ALAN),
    "no-final-newline": (b"name,age\nAda,36\nAlan,41", ADA_ALAN),
    "quoted-comma": (b'name,born\n"Lovelace, Ada",1815\n', [{"name": "Lovelace, Ada", "born": "1815"}]),
    "escaped-quote": (b'id,quote\n1,"She said ""hi""."\n', [{"id": "1", "quote": 'She said "hi".'}]),
    "quoted-header": (b'"first name","last name"\nAda,Lovelace\n', [{"first name": "Ada", "last name": "Lovelace"}]),
    "newline-in-field": (b'id,address\n1,"12 High St\nLeeds"\n', [{"id": "1", "address": "12 High St\nLeeds"}]),
    "crlf-in-field": (b'id,address\r\n1,"12 High St\r\nLeeds"\r\n', [{"id": "1", "address": "12 High St\nLeeds"}]),
    "blank-line-in-field": (b'id,note\n1,"one\n\ntwo"\n', [{"id": "1", "note": "one\n\ntwo"}]),
    "empty-last-line": (b"a,b\n1,2\n\n", [{"a": "1", "b": "2"}]),
    "blank-lines": (b"a,b\n\n1,2\n\n\n3,4\n", [{"a": "1", "b": "2"}, {"a": "3", "b": "4"}]),
    "empty-fields": (b"a,b,c\n,,\n", [{"a": "", "b": "", "c": ""}]),
    "quoted-empty": (b'name\n""\n', [{"name": ""}]),
    "spaces": (b"a,b\n x , y \n", [{"a": " x ", "b": " y "}]),
    "header-only": (b"a,b\n", []),
    "empty-file": (b"", []),
    "bom": (b"\xef\xbb\xbfname,age\nAda,36\n", [{"name": "Ada", "age": "36"}]),
    "non-ascii": (
        "city,country\nZürich,Schweiz\n東京,日本\n".encode(),
        [{"city": "Zürich", "country": "Schweiz"}, {"city": "東京", "country": "日本"}],
    ),
}

INVALID = {
    "too-few-fields": b"a,b\n1\n",
    "too-many-fields": b"a,b\n1,2,3\n",
    "unclosed-quote": b'a,b\n1,"two\n',
    "text-after-quote": b'a,b\n1,"two"x\n',
}


@pytest.mark.parametrize("name", CASES)
def test_cases(tmp_path: Path, name: str) -> None:
    data, want = CASES[name]
    path = tmp_path / "input.csv"
    path.write_bytes(data)
    assert read_csv(path) == want


@pytest.mark.parametrize("name", INVALID)
def test_invalid(tmp_path: Path, name: str) -> None:
    path = tmp_path / "input.csv"
    path.write_bytes(INVALID[name])
    with pytest.raises(csv.Error):
        read_csv(path)


def test_missing(tmp_path: Path) -> None:
    with pytest.raises(FileNotFoundError):
        read_csv(tmp_path / "missing.csv")
