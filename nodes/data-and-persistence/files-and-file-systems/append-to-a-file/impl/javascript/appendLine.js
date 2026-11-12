// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

import { open } from "node:fs/promises";

/** Add line and one "\n" to the end of the file, in one write call. */
export async function appendLine(path, line) {
  if (/[\r\n]/.test(line)) throw new RangeError("the line must not hold \\n or \\r");
  const data = Buffer.from(line + "\n", "utf8");
  // The flag "a" opens the file with O_APPEND and makes it if it is missing.
  // fsPromises.appendFile can call write more than once for one string.
  const file = await open(path, "a");
  try {
    const { bytesWritten } = await file.write(data);
    if (bytesWritten !== data.length) {
      throw new Error(`short write: ${bytesWritten} of ${data.length} bytes`);
    }
  } finally {
    await file.close();
  }
}
