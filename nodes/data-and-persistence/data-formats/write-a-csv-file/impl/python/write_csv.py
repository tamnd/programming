# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-csv-file/.

"""Writes records as a CSV file of RFC 4180."""

import csv
import io


class CsvError(ValueError):
    """A record has more or fewer fields than the first record (the header)."""

    def __init__(self, record: int, got: int, want: int) -> None:
        super().__init__(f"record {record} has {got} fields, the header has {want}")
        self.kind = "field-count"


def _check(records: list[list[str]]) -> None:
    for n, record in enumerate(records):
        if len(record) != len(records[0]):
            raise CsvError(n, len(record), len(records[0]))


def format_csv(records: list[list[str]]) -> str:
    """Returns the CSV text of the records. The first record is the header."""
    _check(records)
    out = io.StringIO(newline="")
    # The default dialect "excel" ends each record with CRLF and quotes a field
    # only when it holds a comma, a double quote, CR or LF.
    csv.writer(out).writerows(records)
    return out.getvalue()


def write_csv(path: str, records: list[list[str]]) -> None:
    """Writes the records to the file at path as UTF-8, with no byte order mark."""
    _check(records)
    # newline="" stops the file object from changing the line breaks.
    with open(path, "w", encoding="utf-8", newline="") as f:
        csv.writer(f).writerows(records)
