// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

import { open } from "node:fs/promises";
import { pipeline } from "node:stream/promises";

/**
 * Copies src to dst through streams with buffers of size bytes, and gives
 * the number of bytes copied. The promise is rejected if a read, a write or
 * the close of dst fails.
 */
export async function copyBuffered(src, dst, size) {
  if (!Number.isInteger(size) || size < 1) {
    throw new RangeError(`size must be an integer of at least 1, not ${size}`);
  }
  // Open src first, so that a missing src does not make an empty dst.
  const input = await open(src, "r");
  let output;
  try {
    output = await open(dst, "w");
  } catch (err) {
    await input.close();
    throw err;
  }
  let total = 0;
  // pipeline waits until dst is written and closed, and it closes both
  // files if one of the steps fails.
  await pipeline(
    input.createReadStream({ highWaterMark: size }),
    async function* (chunks) {
      for await (const chunk of chunks) {
        total += chunk.length;
        yield chunk;
      }
    },
    output.createWriteStream({ highWaterMark: size }),
  );
  return total;
}
