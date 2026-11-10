// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

import { createHash } from "node:crypto";
import { createReadStream } from "node:fs";

// Returns the SHA-256 of the UTF-8 bytes of text, as lowercase hex.
export function sha256Text(text: string): string {
  return createHash("sha256").update(text, "utf8").digest("hex");
}

// Returns the SHA-256 of the bytes of the file at path, as lowercase hex.
// The file is read in chunks, so a large file does not fill the memory.
export async function sha256File(path: string): Promise<string> {
  const hash = createHash("sha256");
  // With no encoding, each chunk is a Buffer of the raw bytes.
  for await (const chunk of createReadStream(path)) {
    hash.update(chunk as Buffer);
  }
  return hash.digest("hex");
}
