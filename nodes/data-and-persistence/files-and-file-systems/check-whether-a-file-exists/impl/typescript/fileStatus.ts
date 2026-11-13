// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

import type { Stats } from "node:fs";
import { lstat, stat } from "node:fs/promises";

/**
 * Tells what is at path: "file", "directory", "other", "missing" or
 * "broken-link". fs.existsSync is not used, because it gives false also for a
 * permission error. Here each error other than "no such entry" is thrown.
 */
export type FileStatus = "file" | "directory" | "other" | "missing" | "broken-link";

export async function fileStatus(path: string): Promise<FileStatus> {
  let info: Stats;
  try {
    info = await stat(path); // follows symbolic links
  } catch (err) {
    const code = (err as NodeJS.ErrnoException).code;
    if (code !== "ENOENT" && code !== "ENOTDIR") throw err;
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
