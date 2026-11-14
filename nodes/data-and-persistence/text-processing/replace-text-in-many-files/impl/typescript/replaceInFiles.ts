// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

import { randomUUID } from "node:crypto";
import { chmodSync, closeSync, fsyncSync, openSync, readFileSync, renameSync, rmSync, statSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";

// Replaces each instance of oldText with newText in each file, and returns the
// number of instances in each file. oldText and newText are plain text, not
// patterns. The code compares the UTF-8 bytes of oldText with the bytes of
// each file, so an instance can contain LF bytes. Instances do not overlap. A
// file with no instance is not written. A changed file goes to a new file in
// the same directory, and renameSync puts it in the place of the old one. The
// files are done in order, and the first error stops the function. Throws a
// RangeError if oldText is empty, and an error whose code is "ENOENT" if a
// file does not exist.
export function replaceInFiles(paths: Iterable<string>, oldText: string, newText: string): number[] {
  if (oldText.length === 0) {
    throw new RangeError("oldText is empty");
  }
  const oldBytes = Buffer.from(oldText, "utf8");
  const newBytes = Buffer.from(newText, "utf8");
  const counts: number[] = [];
  for (const path of paths) {
    const data = readFileSync(path);
    // Copy the bytes between the instances, and newBytes for each instance.
    // The search goes on after the end of each instance.
    const parts: Buffer[] = [];
    let from = 0;
    let at: number;
    while ((at = data.indexOf(oldBytes, from)) !== -1) {
      parts.push(data.subarray(from, at), newBytes);
      from = at + oldBytes.length;
    }
    const n = parts.length / 2;
    if (n > 0) {
      parts.push(data.subarray(from));
      writeAtomically(path, Buffer.concat(parts));
    }
    counts.push(n);
  }
  return counts;
}

// Writes data to a new file next to path, and renames it over path.
function writeAtomically(path: string, data: Buffer): void {
  const tmp = join(dirname(path), `.${basename(path)}.${randomUUID()}.tmp`);
  try {
    // "wx" fails if the file exists, so the code does not write over a file
    // with the same name.
    const fd = openSync(tmp, "wx", 0o600);
    try {
      writeFileSync(fd, data);
      fsyncSync(fd);
    } finally {
      closeSync(fd);
    }
    chmodSync(tmp, statSync(path).mode & 0o7777);
    renameSync(tmp, path);
  } catch (err) {
    rmSync(tmp, { force: true });
    throw err;
  }
}
