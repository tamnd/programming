// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

import { readFileSync } from "node:fs";

// Returns [first, last]: the first count lines and the last count lines of the
// UTF-8 file at path. If the file has fewer than count lines, each array has
// all the lines. A line ends at LF, and the LF is not part of it. A last line
// with no LF is a line too. A missing file throws an error whose code is
// "ENOENT".
export function firstAndLastLines(path, count) {
  const lines = readFileSync(path, "utf8").split("\n");
  // The part after the last LF is empty, or it is a last line with no LF.
  if (lines.at(-1) === "") lines.pop();
  // slice(-count) is not correct: slice(-0) is slice(0), which is all the lines.
  const start = Math.max(lines.length - count, 0);
  return [lines.slice(0, count), lines.slice(start)];
}
