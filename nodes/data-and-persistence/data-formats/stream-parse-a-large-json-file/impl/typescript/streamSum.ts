// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/stream-parse-a-large-json-file/.

import { closeSync, openSync, readSync } from "node:fs";

const CHUNK = 1 << 16; // bytes read at a time

// An element of the array is not an object with a number member amount.
export class BadRecordError extends Error {}

// Yields the source text of each element of the JSON array in the file.
// Node.js has no streaming JSON parser, so this scanner finds where each
// element ends, and JSON.parse then checks and parses the element alone.
function* elementTexts(path: string): Generator<string> {
  const fd = openSync(path, "r");
  try {
    // With stream: true, a UTF-8 sequence split between two chunks is kept for the next call.
    // The decoder also drops a byte order mark at the start.
    const decoder = new TextDecoder("utf-8", { fatal: true });
    const bytes = Buffer.alloc(CHUNK);
    let state = "start"; // start, first (after [), before (after a comma), inside (an element), end
    let text = ""; // the text of the element so far
    let depth = 0;
    let inString = false;
    let escaped = false;
    for (;;) {
      const n = readSync(fd, bytes, 0, CHUNK, null);
      const chunk = decoder.decode(bytes.subarray(0, n), { stream: n > 0 });
      let from = 0; // the start of the current element in chunk
      for (let i = 0; i < chunk.length; i++) {
        const c = chunk[i];
        if (state === "inside") {
          if (inString) {
            if (escaped) escaped = false;
            else if (c === "\\") escaped = true;
            else if (c === '"') inString = false;
            continue;
          }
          if (c === '"') inString = true;
          else if (c === "[" || c === "{") depth++;
          else if (depth > 0 && (c === "]" || c === "}")) depth--;
          else if (depth === 0 && (c === "," || c === "]")) {
            yield text + chunk.slice(from, i);
            text = "";
            state = c === "," ? "before" : "end";
          }
          continue;
        }
        if (c === " " || c === "\t" || c === "\n" || c === "\r") continue;
        if (state === "start" && c === "[") state = "first";
        else if (state === "first" && c === "]") state = "end"; // an empty array
        else if (state === "first" || state === "before") {
          state = "inside";
          from = i;
          i--; // scan this character again as part of the element
        } else throw new SyntaxError(`unexpected ${JSON.stringify(c)} in ${state} state`);
      }
      if (state === "inside") text += chunk.slice(from);
      if (n === 0) break;
    }
    if (state !== "end") throw new SyntaxError("the text is not a complete JSON array");
  } finally {
    closeSync(fd);
  }
}

function isObject(value: unknown): value is Record<string, unknown> {
  return value !== null && typeof value === "object" && !Array.isArray(value);
}

// Returns the number of objects in the JSON array in the file, and the sum of their member amount.
export function streamSum(path: string): [number, number] {
  let count = 0;
  let total = 0;
  for (const text of elementTexts(path)) {
    // JSON.parse throws a SyntaxError for an empty or invalid element.
    const record: unknown = JSON.parse(text);
    const amount = isObject(record) && Object.hasOwn(record, "amount") ? record.amount : undefined;
    if (typeof amount !== "number") {
      throw new BadRecordError(`element ${count} has no number member amount`);
    }
    count++;
    total += amount;
  }
  return [count, total];
}
