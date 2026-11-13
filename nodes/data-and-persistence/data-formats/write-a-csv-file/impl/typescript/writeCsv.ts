// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

// Writes records as a CSV file of RFC 4180. Node.js has no CSV writer, so this file has a small one.

import { writeFileSync } from "node:fs";

/** A record has more or fewer fields than the first record (the header). */
export class CsvError extends Error {
  readonly kind = "field-count";

  constructor(record: number, got: number, want: number) {
    super(`record ${record} has ${got} fields, the header has ${want}`);
    this.name = "CsvError";
  }
}

const SPECIAL = /[",\r\n]/;

function formatField(field: string, alone: boolean): string {
  // A record with one empty field needs quotes, or the line is empty and a reader skips it.
  if (SPECIAL.test(field) || (alone && field === "")) {
    return `"${field.replaceAll('"', '""')}"`;
  }
  return field;
}

/** Returns the CSV text of the records. The first record is the header. */
export function formatCsv(records: readonly (readonly string[])[]): string {
  let out = "";
  for (const [n, record] of records.entries()) {
    if (record.length !== records[0].length) {
      throw new CsvError(n, record.length, records[0].length);
    }
    out += record.map((field) => formatField(field, record.length === 1)).join(",") + "\r\n";
  }
  return out;
}

/** Writes the records to the file at path as UTF-8, with no byte order mark. */
export function writeCsv(path: string, records: readonly (readonly string[])[]): void {
  writeFileSync(path, formatCsv(records), "utf8");
}
