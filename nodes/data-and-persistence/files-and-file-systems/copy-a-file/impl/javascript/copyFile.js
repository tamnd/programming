// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

import { chmod, copyFile as fsCopyFile, stat } from "node:fs/promises";

function fail(code, message, path) {
  const err = new Error(`${code}: ${message}, copy '${path}'`);
  err.code = code;
  err.path = path;
  return err;
}

/**
 * Copies the bytes and the permission bits of the file at src to dst, and
 * replaces a file at dst. fs.copyFile uses the fast copy call of the
 * platform, but it copies a file onto itself with no error, so the code
 * compares the device and inode numbers first. It also gives the error
 * EISDIR for a directory at src or dst. Each error is thrown.
 */
export async function copyFile(src, dst) {
  const s = await stat(src);
  if (s.isDirectory()) {
    throw fail("EISDIR", "illegal operation on a directory", src);
  }
  let d = null;
  try {
    d = await stat(dst);
  } catch (err) {
    if (err.code !== "ENOENT") {
      throw err;
    }
  }
  if (d !== null && d.isDirectory()) {
    throw fail("EISDIR", "illegal operation on a directory", dst);
  }
  if (d !== null && d.dev === s.dev && d.ino === s.ino) {
    throw fail("ESAMEFILE", "src and dst are the same file", dst);
  }
  await fsCopyFile(src, dst);
  await chmod(dst, s.mode & 0o7777);
}
