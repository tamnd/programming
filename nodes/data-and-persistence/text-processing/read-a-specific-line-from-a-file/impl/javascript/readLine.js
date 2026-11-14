// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

import { readFileSync } from "node:fs";

// Returns line number of the UTF-8 file at path, or null if there is no such
// line. Line 1 is the first line. A line ends at LF, and the LF is not part
// of it. A last line with no LF is a line too. A missing file throws an error
// whose code is "ENOENT".
export function readLine(path, number) {
  const lines = readFileSync(path, "utf8").split("\n");
  // The part after the last LF is empty, or it is a last line with no LF.
  if (lines.at(-1) === "") lines.pop();
  // Line numbers start at 1, and indexes start at 0.
  if (!Number.isInteger(number) || number < 1 || number > lines.length) return null;
  return lines[number - 1];
}
