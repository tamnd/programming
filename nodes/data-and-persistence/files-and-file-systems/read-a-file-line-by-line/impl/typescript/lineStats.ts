// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

import { createReadStream } from "node:fs";
import { createInterface } from "node:readline";
import { Readable } from "node:stream";

export interface LineStats {
  lines: number;
  longest: number; // in code points
}

// Decodes chunks of bytes as UTF-8. A bad byte sequence throws a TypeError.
// readline alone would replace it with U+FFFD and go on.
async function* decodeUtf8(chunks: AsyncIterable<Uint8Array>): AsyncGenerator<string> {
  const decoder = new TextDecoder("utf-8", { fatal: true, ignoreBOM: true });
  for await (const chunk of chunks) {
    yield decoder.decode(chunk, { stream: true });
  }
  yield decoder.decode(); // throws if the file ends inside a sequence
}

// A string holds UTF-16 code units; the iterator of a string gives code points.
function codePoints(text: string): number {
  let count = 0;
  for (const _ of text) count++;
  return count;
}

export async function lineStats(path: string): Promise<LineStats> {
  const input = Readable.from(decodeUtf8(createReadStream(path)));
  // crlfDelay: Infinity makes "\r\n" one line break even when a chunk ends between "\r" and "\n".
  const reader = createInterface({ input, crlfDelay: Infinity });
  let lines = 0;
  let longest = 0;
  for await (const line of reader) {
    lines++;
    longest = Math.max(longest, codePoints(line));
  }
  return { lines, longest };
}
