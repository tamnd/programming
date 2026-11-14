// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

import { createReadStream } from "node:fs";

// The 25 code points that have the Unicode property White_Space (PropList.txt).
// The regular expression \s is a different set: it has U+FEFF and not U+0085.
const WHITE_SPACE: ReadonlySet<number> = new Set([
  0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x20, 0x85, 0xa0, 0x1680,
  0x2000, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x2006, 0x2007, 0x2008, 0x2009, 0x200a,
  0x2028, 0x2029, 0x202f, 0x205f, 0x3000,
]);

export interface WordCount {
  lines: number; // the number of LF bytes
  words: number; // runs of one or more code points that are not White_Space
  bytes: number;
}

// Returns the counts for the UTF-8 file at path. Bytes that are not valid
// UTF-8 reject with a TypeError, and a missing file rejects with an error
// whose code is "ENOENT".
export async function wordCount(path: string): Promise<WordCount> {
  // fatal: true makes the decoder throw on bad bytes. ignoreBOM: true keeps a
  // leading U+FEFF as a character, so that each byte is counted.
  const decoder = new TextDecoder("utf-8", { fatal: true, ignoreBOM: true });
  const result: WordCount = { lines: 0, words: 0, bytes: 0 };
  let inWord = false;
  const count = (text: string): void => {
    for (const ch of text) {
      const cp = ch.codePointAt(0)!;
      if (WHITE_SPACE.has(cp)) {
        inWord = false;
        if (cp === 0x0a) result.lines++;
      } else if (!inWord) {
        inWord = true;
        result.words++;
      }
    }
  };
  for await (const chunk of createReadStream(path) as AsyncIterable<Buffer>) {
    result.bytes += chunk.length;
    // stream: true keeps a sequence that the chunk cuts until the next chunk.
    count(decoder.decode(chunk, { stream: true }));
  }
  count(decoder.decode()); // throws if the file ends inside a sequence
  return result;
}
