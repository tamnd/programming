// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

import { readFile } from "node:fs/promises";

/** The error for a file that is not valid CSV. */
export class CsvError extends Error {
  readonly kind: "field-count" | "bad-quote";

  constructor(kind: "field-count" | "bad-quote", message: string) {
    super(message);
    this.name = "CsvError";
    this.kind = kind;
  }
}

/** Returns the length of the line break at i: 1 for LF, 2 for CRLF, 0 for none. */
function lineBreak(text: string, i: number): number {
  if (text[i] === "\n") return 1;
  if (text[i] === "\r" && text[i + 1] === "\n") return 2;
  return 0;
}

/** Splits RFC 4180 text into records of fields. An empty line gives no record. */
export function parseCsv(text: string): string[][] {
  const records: string[][] = [];
  let i = 0;
  while (i < text.length) {
    const empty = lineBreak(text, i);
    if (empty > 0) {
      i += empty;
      continue;
    }
    const record: string[] = [];
    for (;;) {
      let field = "";
      if (text[i] === '"') {
        i++;
        for (;;) {
          if (i >= text.length) throw new CsvError("bad-quote", "a quoted field has no closing quote");
          if (text[i] === '"') {
            if (text[i + 1] !== '"') break;
            field += '"';
            i += 2;
          } else if (lineBreak(text, i) === 2) {
            field += "\n"; // CRLF inside quotes becomes LF.
            i += 2;
          } else {
            field += text[i++];
          }
        }
        i++; // the closing quote
        if (i < text.length && text[i] !== "," && lineBreak(text, i) === 0) {
          throw new CsvError("bad-quote", "text after the closing quote of a field");
        }
      } else {
        const start = i;
        while (i < text.length && text[i] !== "," && lineBreak(text, i) === 0) i++;
        field = text.slice(start, i);
      }
      record.push(field);
      if (text[i] !== ",") break;
      i++;
    }
    i += lineBreak(text, i);
    records.push(record);
  }
  return records;
}

/** Reads a CSV file with a header line into one object for each record. */
export async function readCsv(path: string): Promise<Record<string, string>[]> {
  let text = await readFile(path, "utf8");
  if (text.startsWith("\uFEFF")) text = text.slice(1);
  const [header, ...rows] = parseCsv(text);
  if (header === undefined) return [];
  return rows.map((row, n) => {
    if (row.length !== header.length) {
      throw new CsvError("field-count", `record ${n + 1} has ${row.length} fields, want ${header.length}`);
    }
    // fromEntries makes own properties, also for a name such as "__proto__".
    return Object.fromEntries(header.map((name, k) => [name, row[k] ?? ""]));
  });
}
