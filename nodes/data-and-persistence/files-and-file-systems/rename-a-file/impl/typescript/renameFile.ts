// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

import { rename } from "node:fs/promises";

/**
 * Gives the file at src the name dst, and replaces a file at dst. rename is
 * one call of the operating system, so it is atomic. Across file systems it
 * fails with EXDEV and does not copy. Each error is thrown.
 */
export async function renameFile(src: string, dst: string): Promise<void> {
  await rename(src, dst);
}
