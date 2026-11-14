// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

import { readFileSync } from "node:fs";

// Returns the lines of the UTF-8 file at path, sorted by code point. A line
// ends at LF, and the LF is not part of it. A last line with no LF is a line
// too. A missing file throws an error whose code is "ENOENT".
export function sortLines(path) {
  const data = readFileSync(path);
  const lines = [];
  let start = 0;
  while (start < data.length) {
    let end = data.indexOf(0x0a, start);
    if (end === -1) end = data.length;
    lines.push(data.subarray(start, end));
    start = end + 1;
  }
  // The default sort and < compare UTF-16 code units, which is not the order
  // of the code points. Buffer.compare compares the UTF-8 bytes, which is.
  lines.sort(Buffer.compare);
  return lines.map((line) => line.toString("utf8"));
}
