// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

import { readFileSync } from "node:fs";

// Returns the lines of the UTF-8 file at path that match pattern, a regular
// expression. A line matches if the pattern matches some part of it. A line
// ends at LF, and the LF is not part of it. A last line with no LF is a line
// too. A missing file throws an error whose code is "ENOENT", and a pattern
// that is not valid throws a SyntaxError.
export function filterLines(path: string, pattern: string): string[] {
  // With the s flag, . also matches CR. With the u flag, . matches one code
  // point, not one UTF-16 code unit. There is no g flag, so test() keeps no
  // state from one line to the next.
  const regex = new RegExp(pattern, "su");
  const lines = readFileSync(path, "utf8").split("\n");
  // The part after the last LF is empty, or it is a last line with no LF.
  if (lines.at(-1) === "") lines.pop();
  return lines.filter((line) => regex.test(line));
}
