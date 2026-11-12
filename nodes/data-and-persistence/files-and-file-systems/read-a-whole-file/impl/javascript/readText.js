// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

import { readFile } from "node:fs/promises";

// fatal: true rejects bad bytes; the default puts U+FFFD in their place.
// ignoreBOM: true keeps a byte order mark as U+FEFF; the default drops it.
const decoder = new TextDecoder("utf-8", { fatal: true, ignoreBOM: true });

/** Return all of the text of a UTF-8 file, with no change to it. */
export async function readText(path) {
  const bytes = await readFile(path);
  return decoder.decode(bytes);
}
