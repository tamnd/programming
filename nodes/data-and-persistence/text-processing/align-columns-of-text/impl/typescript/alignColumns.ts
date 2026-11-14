// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

export type Align = "left" | "right" | "center";

const ALIGNS: readonly string[] = ["left", "right", "center"];

// Returns text with its fields in columns. A line ends at LF. Each $ in a line
// separates two fields, and a $ at the end of a line does not start a new
// field. An empty line has no fields. The width of a column is the largest
// number of code points of a field in it. Each field is padded with spaces to
// the width of its column, on the right (left), on the left (right), or on the
// two sides (center), with the odd space on the right. One space separates two
// columns. The padding on the right of the last field of a line is not
// written. Each line of the result ends with LF. Throws a RangeError if align
// is not left, right or center.
export function alignColumns(text: string, align: Align): string {
  if (!ALIGNS.includes(align)) {
    throw new RangeError(`bad align: ${align}`);
  }
  const lines = text.split("\n");
  if (lines.at(-1) === "") {
    // A final LF ends the last line. It does not start a new line.
    lines.pop();
  }
  const rows = lines.map(splitFields);
  const widths: number[] = [];
  for (const row of rows) {
    row.forEach((field, j) => {
      widths[j] = Math.max(widths[j] ?? 0, codePoints(field));
    });
  }
  let out = "";
  for (const row of rows) {
    const parts = row.map((field, j) => {
      const pad = widths[j] - codePoints(field);
      const left = align === "left" ? 0 : align === "right" ? pad : Math.floor(pad / 2);
      const right = j < row.length - 1 ? pad - left : 0;
      return " ".repeat(left) + field + " ".repeat(right);
    });
    out += parts.join(" ") + "\n";
  }
  return out;
}

// Returns the fields of a line: none for an empty line.
function splitFields(line: string): string[] {
  if (line === "") {
    return [];
  }
  const fields = line.split("$");
  if (line.endsWith("$")) {
    fields.pop();
  }
  return fields;
}

// length counts UTF-16 code units, so a character outside the BMP counts as 2.
// The string iterator gives code points.
function codePoints(s: string): number {
  let n = 0;
  for (const _ of s) {
    n++;
  }
  return n;
}
