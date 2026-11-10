// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

import { createInterface } from "node:readline";

/**
 * Reads a stream to its end and returns its lines without "\n" or "\r\n".
 * @param {import("node:stream").Readable} [input] standard input by default
 * @returns {Promise<string[]>}
 */
export async function readLines(input = process.stdin) {
  // Decode the bytes as UTF-8, also when a character is split between two chunks.
  input.setEncoding("utf8");
  // crlfDelay: Infinity makes "\r\n" one line break even when a chunk ends between "\r" and "\n".
  const reader = createInterface({ input, crlfDelay: Infinity });
  const lines = [];
  // The loop ends when the stream ends.
  for await (const line of reader) {
    lines.push(line);
  }
  return lines;
}
