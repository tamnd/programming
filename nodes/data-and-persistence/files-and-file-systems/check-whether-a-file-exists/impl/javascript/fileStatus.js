// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

import { lstat, stat } from "node:fs/promises";

/**
 * Tells what is at path: "file", "directory", "other", "missing" or
 * "broken-link". fs.existsSync is not used, because it gives false also for a
 * permission error. Here each error other than "no such entry" is thrown.
 */
export async function fileStatus(path) {
  let info;
  try {
    info = await stat(path); // follows symbolic links
  } catch (err) {
    if (err.code !== "ENOENT" && err.code !== "ENOTDIR") throw err;
    try {
      if ((await lstat(path)).isSymbolicLink()) return "broken-link";
    } catch {
      // There is no entry at all.
    }
    return "missing";
  }
  if (info.isFile()) return "file";
  if (info.isDirectory()) return "directory";
  return "other";
}
