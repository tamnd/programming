# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-csv-to-json/.

"""Converts CSV text with a header line to a JSON array of objects."""

import csv
import io
import json


class CsvError(ValueError):
    """The text is not valid CSV. kind is "field-count" or "bad-quote"."""

    def __init__(self, kind: str, message: str) -> None:
        super().__init__(message)
        self.kind = kind


def csv_to_json(text: str) -> str:
    """Returns a JSON array with one object for each record after the header."""
    # newline="" gives the line breaks inside quoted fields to the csv module unchanged.
    # strict=True raises csv.Error on a bad quote. The default accepts it.
    reader = csv.reader(io.StringIO(text, newline=""), strict=True)
    header = None
    records = []
    try:
        for row in reader:
            if not row:
                continue  # an empty line
            if header is None:
                header = row
                continue
            if len(row) != len(header):
                raise CsvError("field-count", f"line {reader.line_num}: {len(row)} fields, want {len(header)}")
            # A dict keeps the order of insertion, so the members keep the order of the header.
            records.append({name: field.replace("\r\n", "\n") for name, field in zip(header, row)})
    except csv.Error as e:
        raise CsvError("bad-quote", str(e)) from e
    # ensure_ascii=False writes non-ASCII characters as themselves.
    # The separators remove the spaces after "," and ":".
    return json.dumps(records, ensure_ascii=False, separators=(",", ":"))
