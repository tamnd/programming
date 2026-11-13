// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

import { unlink } from "node:fs/promises";

/**
 * Deletes the file at path. Gives true if a file was there, false if not.
 * unlink does not delete a directory, and it deletes a symbolic link, not its
 * target. Each error other than "no such entry" is thrown, so a permission
 * error is not taken for a missing file.
 */
export async function deleteFile(path: string): Promise<boolean> {
  try {
    await unlink(path);
  } catch (err) {
    const code = (err as NodeJS.ErrnoException).code;
    if (code === "ENOENT" || code === "ENOTDIR") {
      return false;
    }
    throw err;
  }
  return true;
}
