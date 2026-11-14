// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

import { randomUUID } from "node:crypto";
import { chmodSync, closeSync, fsyncSync, openSync, readFileSync, renameSync, rmSync, statSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";

// Returns the offset just after the line that starts at offset: after the
// next LF, or at the end of the data if there is no LF.
function nextLine(data, offset) {
  const lf = data.indexOf(10, offset);
  return lf === -1 ? data.length : lf + 1;
}

// Removes count lines from the file at path, from line start, and returns how
// many it removed. Line 1 is the first line. A line ends at LF, and the LF
// goes with the line. The result is less than count if the file ends first.
// If no line goes, the file is not written. Otherwise the rest of the file
// goes to a new file in the same directory, and renameSync puts it in the
// place of the old one. Throws a RangeError if start is less than 1 or count
// is less than 0, and an error whose code is "ENOENT" if the file does not
// exist.
export function removeLines(path, start, count) {
  if (!Number.isInteger(start) || !Number.isInteger(count) || start < 1 || count < 0) {
    throw new RangeError(`bad range: start ${start}, count ${count}`);
  }
  // A Buffer, not a string: the bytes do not change.
  const data = readFileSync(path);
  let offset = 0;
  for (let number = 1; number < start && offset < data.length; number++) {
    offset = nextLine(data, offset);
  }
  const from = offset;
  let removed = 0;
  while (removed < count && offset < data.length) {
    offset = nextLine(data, offset);
    removed++;
  }
  if (removed === 0) return 0;
  const tmp = join(dirname(path), `.${basename(path)}.${randomUUID()}.tmp`);
  try {
    // "wx" fails if the file exists, so the code does not write over a file
    // with the same name.
    const fd = openSync(tmp, "wx", 0o600);
    try {
      writeFileSync(fd, Buffer.concat([data.subarray(0, from), data.subarray(offset)]));
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
  return removed;
}
