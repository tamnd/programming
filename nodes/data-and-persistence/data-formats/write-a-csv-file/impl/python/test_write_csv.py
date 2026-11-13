# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-csv-file/.

import csv
from pathlib import Path

import pytest

from write_csv import CsvError, format_csv, write_csv

# name: (records, the CSV text)
CASES = {
    "simple": ([["name", "age"], ["Ada", "36"], ["Alan", "41"]], "name,age\r\nAda,36\r\nAlan,41\r\n"),
    "comma": ([["name", "born"], ["Lovelace, Ada", "1815"]], "name,born\r\n\"Lovelace, Ada\",1815\r\n"),
    "double-quote": ([["id", "quote"], ["1", "She said \"hi\"."]], "id,quote\r\n1,\"She said \"\"hi\"\".\"\r\n"),
    "only-a-quote": ([["q"], ["\""]], "q\r\n\"\"\"\"\r\n"),
    "header-only": ([["name", "age"]], "name,age\r\n"),
    "newline-in-field": ([["id", "address"], ["1", "12 High St\nLeeds"]], "id,address\r\n1,\"12 High St\nLeeds\"\r\n"),
    "crlf-in-field": ([["id", "address"], ["1", "12 High St\r\nLeeds"]], "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n"),
    "cr-in-field": ([["id", "text"], ["1", "a\rb"]], "id,text\r\n1,\"a\rb\"\r\n"),
    "empty-fields": ([["a", "b", "c"], ["", "", ""]], "a,b,c\r\n,,\r\n"),
    "one-empty-field": ([["note"], [""], ["x"]], "note\r\n\"\"\r\nx\r\n"),
    "spaces": ([["a", "b"], [" x ", " y"]], "a,b\r\n x , y\r\n"),
    "no-records": ([], ""),
    "non-ascii": ([["city"], ["Zürich"], ["東京"]], "city\r\nZürich\r\n東京\r\n"),
}

ERRORS = {
    "too-many-fields": [["a", "b"], ["1", "2", "3"]],
    "too-few-fields": [["a", "b"], ["1"]],
}


@pytest.mark.parametrize("name", CASES)
def test_format(name: str) -> None:
    records, want = CASES[name]
    assert format_csv(records) == want


@pytest.mark.parametrize("name", CASES)
def test_write_and_read_back(name: str, tmp_path: Path) -> None:
    records, want = CASES[name]
    path = tmp_path / "out.csv"
    write_csv(str(path), records)
    assert path.read_bytes() == want.encode("utf-8")
    with open(path, encoding="utf-8", newline="") as f:
        assert list(csv.reader(f)) == records


@pytest.mark.parametrize("name", ERRORS)
def test_field_count(name: str, tmp_path: Path) -> None:
    with pytest.raises(CsvError) as e:
        format_csv(ERRORS[name])
    assert e.value.kind == "field-count"
    path = tmp_path / "out.csv"
    with pytest.raises(CsvError):
        write_csv(str(path), ERRORS[name])
    assert not path.exists()
