// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

import { readFileSync } from "node:fs";

const HUNK = /^@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@/;

/**
 * The patch is malformed, or a hunk does not match the text. kind is
 * "malformed" or "mismatch", and line is the number of the line of the patch
 * where the error is.
 */
export type PatchErrorKind = "malformed" | "mismatch";

export class PatchError extends Error {
  readonly kind: PatchErrorKind;
  readonly line: number;

  constructor(kind: PatchErrorKind, line: number) {
    super(`${kind} at line ${line} of the patch`);
    this.name = "PatchError";
    this.kind = kind;
    this.line = line;
  }
}

/** Splits text after each LF. Each line keeps its LF, and a last line with no LF is a line too. */
export function splitLines(text: string): string[] {
  const lines: string[] = [];
  let start = 0;
  while (start < text.length) {
    let end = text.indexOf("\n", start);
    end = end < 0 ? text.length : end + 1;
    lines.push(text.slice(start, end));
    start = end;
  }
  return lines;
}

/**
 * Returns the text that the unified diff patch makes from the text oldText.
 * Each hunk must match the lines of oldText at the start of its old range.
 * Throws PatchError if the patch is malformed or a hunk does not match.
 */
export function applyPatch(oldText: string, patch: string): string {
  const lines = splitLines(oldText);
  // The lines of the patch, with no LF. A CR stays in its line.
  const p = splitLines(patch).map((l) => (l.endsWith("\n") ? l.slice(0, -1) : l));
  const out: string[] = [];
  let cur = 0; // the index of the next line of oldText to use
  let hunks = 0;
  let i = 0;
  while (i < p.length) {
    if (!p[i].startsWith("@@")) {
      i++; // a header line, or another line outside a hunk
      continue;
    }
    const m = HUNK.exec(p[i]);
    if (m === null) throw new PatchError("malformed", i + 1);
    hunks++;
    const oldStart = Number(m[1]);
    const oldCount = m[2] === undefined ? 1 : Number(m[2]);
    const newCount = m[4] === undefined ? 1 : Number(m[4]);
    // An empty range starts after the line oldStart.
    const pos = oldCount > 0 ? oldStart - 1 : oldStart;
    if (pos < 0) throw new PatchError("malformed", i + 1);
    if (pos < cur || pos > lines.length) throw new PatchError("mismatch", i + 1);
    for (; cur < pos; cur++) out.push(lines[cur]);
    i++;
    // The mark, the text, the number in the patch and the end of each line:
    // an LF, or "" before a backslash line.
    const body: { mark: string; text: string; number: number; end: string }[] = [];
    let oldLeft = oldCount;
    let newLeft = newCount;
    while (oldLeft > 0 || newLeft > 0) {
      if (i === p.length) throw new PatchError("malformed", i);
      const line = p[i];
      const mark = line === "" ? " " : line[0];
      if (mark === "\\") {
        if (body.length === 0) throw new PatchError("malformed", i + 1);
        body[body.length - 1].end = ""; // the line before has no LF
        i++;
        continue;
      }
      if (!" -+".includes(mark)) throw new PatchError("malformed", i + 1);
      if (mark !== "+") oldLeft--;
      if (mark !== "-") newLeft--;
      if (oldLeft < 0 || newLeft < 0) throw new PatchError("malformed", i + 1);
      body.push({ mark, text: line.slice(1), number: i + 1, end: "\n" });
      i++;
    }
    if (i < p.length && p[i].startsWith("\\") && body.length > 0) {
      body[body.length - 1].end = "";
      i++;
    }
    for (const { mark, text, number, end } of body) {
      const line = text + end;
      if (mark !== "+") {
        if (cur === lines.length || lines[cur] !== line) throw new PatchError("mismatch", number);
        cur++;
      }
      if (mark !== "-") out.push(line);
    }
  }
  if (hunks === 0) throw new PatchError("malformed", p.length);
  for (; cur < lines.length; cur++) out.push(lines[cur]);
  return out.join("");
}

/**
 * Returns applyPatch of the two UTF-8 files. Throws an error with the code
 * ENOENT if a file does not exist, and PatchError.
 */
export function applyPatchFile(path: string, patchPath: string): string {
  return applyPatch(readFileSync(path, "utf8"), readFileSync(patchPath, "utf8"));
}
