// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

import { chmod, stat } from "node:fs/promises";

/**
 * Sets the mode of the file at path, and returns the new permission bits.
 * mode is three or four octal digits, as for the chmod command. Without the
 * m flag, $ matches only at the end of the string, so "640\n" is not valid.
 * chmod follows a symbolic link and sets the mode as it is: the umask does
 * not apply. The result is read back with stat, because the system can
 * ignore some bits.
 */
export async function setMode(path: string, mode: string): Promise<number> {
  if (typeof mode !== "string" || !/^[0-7]{3,4}$/.test(mode)) {
    throw new RangeError(`invalid mode: ${JSON.stringify(mode)}`);
  }
  await chmod(path, parseInt(mode, 8));
  return (await stat(path)).mode & 0o7777;
}
