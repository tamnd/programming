// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

import fs from "node:fs";

// Follows a file that grows, as tail -F does, one poll at a time. The
// follower keeps the file open, so it can read the rest of the file after a
// rename or a delete. It starts at the end of the file. The constructor
// throws an error with the code ENOENT if no file is at path.
export class Follower {
  path: string;
  fd: number | null;
  offset: number;
  pending: Buffer; // the bytes after the last LF

  constructor(path: string) {
    this.path = path;
    this.fd = fs.openSync(path, "r");
    this.offset = fs.fstatSync(this.fd).size;
    this.pending = Buffer.alloc(0);
  }

  // Returns the complete lines that were added since the last poll.
  poll(): string[] {
    const lines: string[] = [];
    if (this.fd !== null) {
      // Look at the path before the read. Then the read gets each byte that
      // was written before a rename or a delete.
      const now = fs.statSync(this.path, { bigint: true, throwIfNoEntry: false });
      const open = fs.fstatSync(this.fd, { bigint: true });
      if (open.size < BigInt(this.offset)) {
        // The file is shorter than the offset, so it was truncated.
        this.#flush(lines);
        this.offset = 0;
      }
      this.#read(lines);
      if (now === undefined || now.dev !== open.dev || now.ino !== open.ino) {
        // The file was deleted or renamed. Its rest is read.
        this.#flush(lines);
        this.close();
      }
    }
    if (this.fd === null) {
      try {
        this.fd = fs.openSync(this.path, "r");
      } catch (err) {
        if ((err as NodeJS.ErrnoException).code === "ENOENT") {
          return lines;
        }
        throw err;
      }
      this.offset = 0;
      this.#read(lines);
    }
    return lines;
  }

  close(): void {
    if (this.fd !== null) {
      fs.closeSync(this.fd);
      this.fd = null;
    }
  }

  #read(lines: string[]): void {
    const chunks = [this.pending];
    const buf = Buffer.alloc(65536);
    for (;;) {
      const n = fs.readSync(this.fd as number, buf, 0, buf.length, this.offset);
      if (n === 0) {
        break;
      }
      chunks.push(Buffer.from(buf.subarray(0, n)));
      this.offset += n;
    }
    // Split the bytes at each LF first, and decode each line after. A chunk
    // can end in the middle of a UTF-8 character.
    const data = Buffer.concat(chunks);
    let start = 0;
    for (let lf = data.indexOf(10); lf >= 0; lf = data.indexOf(10, start)) {
      lines.push(data.toString("utf8", start, lf));
      start = lf + 1;
    }
    this.pending = data.subarray(start);
  }

  #flush(lines: string[]): void {
    if (this.pending.length > 0) {
      lines.push(this.pending.toString("utf8"));
      this.pending = Buffer.alloc(0);
    }
  }
}

// Writes initial to path, makes a follower, does the steps and returns the
// lines of each poll.
export function followFile(path: string, initial: string, steps: string[]): string[][] {
  fs.writeFileSync(path, initial);
  const follower = new Follower(path);
  const old = `${path}.1`;
  const polls: string[][] = [];
  try {
    for (const step of steps) {
      const colon = step.indexOf(":");
      const op = colon < 0 ? step : step.slice(0, colon);
      const text = colon < 0 ? "" : step.slice(colon + 1);
      if (op === "poll") {
        polls.push(follower.poll());
      } else if (op === "append") {
        fs.appendFileSync(path, text);
      } else if (op === "append-old") {
        fs.appendFileSync(old, text);
      } else if (op === "truncate") {
        fs.writeFileSync(path, text);
      } else if (op === "rotate") {
        fs.renameSync(path, old);
        fs.writeFileSync(path, "", { flag: "wx" });
      } else if (op === "delete") {
        fs.unlinkSync(path);
      } else if (op === "create") {
        fs.writeFileSync(path, text, { flag: "wx" });
      } else {
        throw new RangeError(`bad step: ${step}`);
      }
    }
  } finally {
    follower.close();
  }
  return polls;
}
