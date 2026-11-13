// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

import { stat } from "node:fs/promises";

const NS_PER_SECOND = 1_000_000_000n;

/**
 * Returns the modification time of the file at path, in whole seconds since
 * 1970-01-01T00:00:00Z. stat follows a symbolic link. With bigint: true, the
 * time is an exact count of nanoseconds (mtimeNs). The division of two
 * bigints rounds toward zero, so the code subtracts 1 for a time before 1970
 * with a part of a second. A missing file gives an error with the code ENOENT.
 */
export async function fileMtime(path) {
  const ns = (await stat(path, { bigint: true })).mtimeNs;
  let s = ns / NS_PER_SECOND;
  if (ns % NS_PER_SECOND < 0n) s -= 1n;
  return Number(s);
}
