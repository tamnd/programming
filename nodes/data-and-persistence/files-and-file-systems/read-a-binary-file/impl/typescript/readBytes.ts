// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

import { open, readFile } from "node:fs/promises";

/** The error of readExact when the file ends before byte n. */
export class UnexpectedEofError extends Error {
  constructor(path: string, got: number, n: number) {
    super(`${path}: ${got} of ${n} bytes before the end of the file`);
    this.name = "UnexpectedEofError";
  }
}

/** Return all of the bytes of a file as a Buffer, with no decoding. */
export async function readBytes(path: string): Promise<Buffer> {
  // With no encoding option, readFile gives a Buffer and not a string.
  return readFile(path);
}

/** Return the first n bytes of a file, or throw UnexpectedEofError if it is shorter. */
export async function readExact(path: string, n: number): Promise<Buffer> {
  const buffer = Buffer.alloc(n);
  const file = await open(path, "r");
  try {
    let got = 0;
    while (got < n) {
      // One read can give fewer bytes than it asks for, so read again.
      const { bytesRead } = await file.read(buffer, got, n - got, null);
      if (bytesRead === 0) throw new UnexpectedEofError(path, got, n);
      got += bytesRead;
    }
    return buffer;
  } finally {
    await file.close();
  }
}
