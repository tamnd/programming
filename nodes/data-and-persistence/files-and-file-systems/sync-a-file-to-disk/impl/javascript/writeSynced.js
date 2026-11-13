// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

import { closeSync, fsyncSync, openSync, writeFileSync } from "node:fs";
import { dirname, resolve } from "node:path";

/**
 * Syncs the entries of a directory, so that a new name in it is on disk.
 * @param {string} dir
 */
export function syncDirectory(dir) {
  const fd = openSync(dir, "r");
  try {
    fsyncSync(fd);
  } finally {
    closeSync(fd);
  }
}

/**
 * Writes text to file as UTF-8, and returns when it is on the disk.
 * @param {string} file
 * @param {string} text
 */
export function writeSynced(file, text) {
  // The flag "w" creates or truncates the file.
  const fd = openSync(file, "w");
  try {
    // With a descriptor, writeFileSync writes all the bytes, with no buffer
    // in Node.js, so fsync sees all of them.
    writeFileSync(fd, text, "utf8");
    fsyncSync(fd);
  } finally {
    closeSync(fd);
  }
  syncDirectory(dirname(resolve(file)));
}
