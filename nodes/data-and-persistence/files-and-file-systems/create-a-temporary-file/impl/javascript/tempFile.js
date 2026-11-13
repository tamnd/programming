// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

import { randomBytes } from "node:crypto";
import { open, rm } from "node:fs/promises";
import { join } from "node:path";

// Node.js has mkdtemp for a directory, but no call that makes a temporary
// file. So makeTempFile makes the name from random bytes, and opens it with
// the flag "wx", which fails when the path exists (O_CREAT | O_EXCL). It tries
// a new name when the name is taken, and it gives the mode 0o600.
export async function makeTempFile(dir, prefix) {
  if (prefix.includes("/")) {
    throw new RangeError(`invalid prefix: ${JSON.stringify(prefix)}`);
  }
  for (let attempt = 0; attempt < 100; attempt++) {
    const path = join(dir, prefix + randomBytes(6).toString("hex"));
    let handle;
    try {
      handle = await open(path, "wx", 0o600);
    } catch (err) {
      if (err.code === "EEXIST") continue;
      throw err;
    }
    await handle.close();
    return path;
  }
  throw new Error("no free temporary name after 100 attempts");
}

export async function useTempFile(dir, prefix, data) {
  const path = await makeTempFile(dir, prefix);
  try {
    const handle = await open(path, "r+");
    try {
      await handle.writeFile(data, "utf8");
      const buf = Buffer.alloc((await handle.stat()).size);
      await handle.read(buf, 0, buf.length, 0);
      return buf.toString("utf8");
    } finally {
      await handle.close();
    }
  } finally {
    await rm(path);
  }
}
