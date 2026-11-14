// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

import { createReadStream } from "node:fs";

const LF = 0x0a;

export interface LineCount {
  newlines: number; // the number of LF bytes, as wc -l prints it
  lines: number; // also counts a last line that has no LF
}

// Counts the lines of the file at path without decoding it. The stream gives
// Buffers, because no encoding is set. A missing file rejects with an error
// whose code is "ENOENT".
export async function countLines(path: string): Promise<LineCount> {
  let newlines = 0;
  let last = LF; // an empty file has no line without an LF
  for await (const chunk of createReadStream(path) as AsyncIterable<Buffer>) {
    for (let i = chunk.indexOf(LF); i !== -1; i = chunk.indexOf(LF, i + 1)) {
      newlines++;
    }
    if (chunk.length > 0) {
      last = chunk[chunk.length - 1];
    }
  }
  return { newlines, lines: newlines + (last === LF ? 0 : 1) };
}
