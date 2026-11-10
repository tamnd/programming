// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

import { randomUUID } from "node:crypto";
import { open, rename, rm, writeFile } from "node:fs/promises";
import { basename, dirname, join } from "node:path";

/**
 * Writes text to file as UTF-8, with no change to the line ends.
 * @param {string} file
 * @param {string} text
 * @param {"truncate" | "append" | "atomic"} mode
 * @returns {Promise<void>}
 */
export async function writeText(file, text, mode) {
  if (mode === "truncate" || mode === "append") {
    // The flag "w" truncates or creates the file; "a" appends or creates it.
    await writeFile(file, text, { encoding: "utf8", flag: mode === "truncate" ? "w" : "a" });
    return;
  }
  if (mode !== "atomic") throw new RangeError(`unknown mode ${mode}`);
  // Node.js has no function that makes a temporary file. "wx" fails if the name exists.
  const tmp = join(dirname(file), `.${basename(file)}.${randomUUID()}.tmp`);
  const handle = await open(tmp, "wx");
  try {
    try {
      await handle.writeFile(text, "utf8");
      await handle.sync();
    } finally {
      await handle.close();
    }
    await rename(tmp, file);
  } catch (error) {
    await rm(tmp, { force: true });
    throw error;
  }
}
