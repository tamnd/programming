// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

import { stat } from "node:fs/promises";

/**
 * Returns the size in bytes of the regular file at path. stat follows a
 * symbolic link, so a link gives the size of its target. A missing file
 * gives an error with the code ENOENT. stat also gives a size for a
 * directory, but it is not the size of a file, so the code throws an error
 * with the code EISDIR.
 */
export async function fileSize(path: string): Promise<number> {
  const st = await stat(path);
  if (st.isDirectory()) {
    throw Object.assign(new Error(`EISDIR: illegal operation on a directory, stat '${path}'`), {
      code: "EISDIR",
      path,
    });
  }
  return st.size;
}
