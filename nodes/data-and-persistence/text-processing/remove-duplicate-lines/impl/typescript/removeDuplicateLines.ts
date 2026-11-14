// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

import { readFileSync } from "node:fs";

// Returns the lines of the UTF-8 file at path, each one only once, at the
// place of its first copy. A line ends at LF, and the LF is not part of it. A
// last line with no LF is a line too. A missing file throws an error whose
// code is "ENOENT".
export function removeDuplicateLines(path: string): string[] {
  // readline also ends a line at a CR, so the code splits the bytes itself.
  const data = readFileSync(path);
  // A Set keeps its values in the order in which they were first added.
  const unique = new Set<string>();
  let start = 0;
  while (start < data.length) {
    let end = data.indexOf(0x0a, start);
    if (end === -1) end = data.length;
    unique.add(data.toString("utf8", start, end));
    start = end + 1;
  }
  return [...unique];
}
