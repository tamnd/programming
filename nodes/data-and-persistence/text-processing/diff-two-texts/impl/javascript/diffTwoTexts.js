// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

import { readFileSync } from "node:fs";

/** Splits text at each LF. A final LF does not start a new line. */
export function splitLines(text) {
  const lines = text.split("\n");
  // "a\n".split("\n") is ["a", ""], and "".split("\n") is [""].
  if (lines[lines.length - 1] === "") lines.pop();
  return lines;
}

/**
 * Returns the edit script that changes the lines of oldText into the lines of
 * newText. Each line of the result is a space and a line that stays, a - and a
 * line of oldText that goes, or a + and a line of newText that comes, with an
 * LF at the end. The script has the smallest number of - and + lines. When two
 * scripts are equally short, a - comes before a +.
 */
export function diffLines(oldText, newText) {
  const a = splitLines(oldText);
  const b = splitLines(newText);
  const n = a.length;
  const m = b.length;
  const w = m + 1;
  // lcs[i * w + j] is the length of a longest common subsequence of the lines
  // from a[i] and from b[j]. Row n and column m stay 0.
  const lcs = new Int32Array((n + 1) * w);
  for (let i = n - 1; i >= 0; i--) {
    for (let j = m - 1; j >= 0; j--) {
      lcs[i * w + j] =
        a[i] === b[j] ? lcs[(i + 1) * w + j + 1] + 1 : Math.max(lcs[(i + 1) * w + j], lcs[i * w + j + 1]);
    }
  }
  const out = [];
  let i = 0;
  let j = 0;
  while (i < n && j < m) {
    if (a[i] === b[j]) {
      out.push(` ${a[i++]}\n`);
      j++;
    } else if (lcs[(i + 1) * w + j] >= lcs[i * w + j + 1]) {
      out.push(`-${a[i++]}\n`);
    } else {
      out.push(`+${b[j++]}\n`);
    }
  }
  while (i < n) out.push(`-${a[i++]}\n`);
  while (j < m) out.push(`+${b[j++]}\n`);
  return out.join("");
}

/**
 * Returns diffLines of the two UTF-8 files. Throws an error with the code
 * ENOENT if a file does not exist.
 */
export function diffFiles(oldPath, newPath) {
  return diffLines(readFileSync(oldPath, "utf8"), readFileSync(newPath, "utf8"));
}
