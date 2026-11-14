// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

import { createReadStream } from "node:fs";
import { createInterface } from "node:readline";

// Returns the n most frequent words of the file at path, as [word, count]
// pairs. A word is a run of ASCII letters, changed to lower case. The pairs are
// sorted by count from high to low, then by word. A missing file rejects with
// an error whose code is "ENOENT".
export async function wordFrequencies(path, n) {
  const counts = new Map();
  // latin1 maps each byte to one character, so the file needs no decoding, and
  // each byte that is not an ASCII letter ends a word. LF is not a letter, so
  // no word goes across two lines.
  const lines = createInterface({ input: createReadStream(path, { encoding: "latin1" }), crlfDelay: Infinity });
  for await (const line of lines) {
    for (const [match] of line.matchAll(/[A-Za-z]+/g)) {
      const word = match.toLowerCase();
      counts.set(word, (counts.get(word) ?? 0) + 1);
    }
  }
  // < and > compare code units and do not use the locale.
  const ranked = [...counts].sort(([a, x], [b, y]) => y - x || (a < b ? -1 : a > b ? 1 : 0));
  return ranked.slice(0, n);
}
