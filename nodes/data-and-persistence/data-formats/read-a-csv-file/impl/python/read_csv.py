# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-csv-file/.

import csv
import os


def read_csv(path: str | os.PathLike[str]) -> list[dict[str, str]]:
    """Read a CSV file with a header line into one dict for each record.

    Raise csv.Error for a bad quote or for a record with the wrong number of
    fields.
    """
    # utf-8-sig removes a byte order mark. newline="" gives the line breaks
    # inside quoted fields to the csv module unchanged.
    with open(path, encoding="utf-8-sig", newline="") as file:
        # strict=True raises csv.Error on a bad quote. The default accepts it.
        reader = csv.reader(file, strict=True)
        header = next(reader, None)
        while header == []:
            header = next(reader, None)
        if header is None:
            return []
        records = []
        for row in reader:
            if not row:
                continue  # an empty line
            if len(row) != len(header):
                raise csv.Error(f"line {reader.line_num}: {len(row)} fields, want {len(header)}")
            records.append({name: field.replace("\r\n", "\n") for name, field in zip(header, row)})
        return records
