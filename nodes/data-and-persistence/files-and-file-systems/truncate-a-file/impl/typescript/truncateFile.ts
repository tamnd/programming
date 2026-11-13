// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

import { truncate } from "node:fs/promises";

/**
 * Sets the size of the existing file at path to length bytes. truncate cuts
 * the bytes after length, or adds zero bytes up to length, and it does not
 * make a missing file. It uses 0 for a negative length, so the code throws a
 * RangeError first. Each other error, such as ENOENT or EISDIR, is thrown.
 */
export async function truncateFile(path: string, length: number): Promise<void> {
  if (!Number.isSafeInteger(length) || length < 0) {
    throw new RangeError(`length must be an integer, 0 or more: ${length}`);
  }
  await truncate(path, length);
}
