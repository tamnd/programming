// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

import { readdir } from "node:fs/promises";
import { join } from "node:path";

/**
 * Returns the relative paths of the entries under root. A directory has a "/"
 * at the end. A symbolic link is listed, but it is never followed.
 * @param {string} root
 * @param {boolean} recursive
 * @returns {Promise<string[]>}
 */
export async function listEntries(root, recursive) {
  const out = [];
  async function walk(dir, prefix) {
    for (const entry of await readdir(dir, { withFileTypes: true })) {
      // isDirectory is false for a symbolic link, also for a link to a directory.
      if (entry.isDirectory()) {
        out.push(`${prefix}${entry.name}/`);
        if (recursive) await walk(join(dir, entry.name), `${prefix}${entry.name}/`);
      } else {
        out.push(prefix + entry.name);
      }
    }
  }
  await walk(root, "");
  // The default sort compares UTF-16 code units. That is code point order
  // for names with no characters outside the Basic Multilingual Plane.
  return out.sort();
}
