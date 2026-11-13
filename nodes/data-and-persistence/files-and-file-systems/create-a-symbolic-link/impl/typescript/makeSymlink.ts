// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

import { readlink, symlink } from "node:fs/promises";

/**
 * Makes a symbolic link at link that holds target, and returns the target
 * that the link holds, read back with readlink. target is stored as it is:
 * it need not exist, and a relative target is read from the directory of
 * link. The third argument of symlink, the type, is read only on Windows.
 * symlink rejects with EEXIST when link exists, also as a dangling link,
 * and with ENOENT when the directory of link does not exist.
 */
export async function makeSymlink(target: string, link: string): Promise<string> {
  await symlink(target, link);
  return readlink(link);
}
