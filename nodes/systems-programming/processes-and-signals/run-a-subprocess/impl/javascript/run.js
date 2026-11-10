// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

import { spawnSync } from "node:child_process";

/**
 * Runs argv with no shell and returns [status, stdout, stderr].
 * A missing program throws an Error with the code "ENOENT".
 * @param {string[]} argv
 * @returns {[number, string, string]}
 */
export function run(argv) {
  const [file, ...args] = argv;
  const result = spawnSync(file, args, {
    stdio: ["ignore", "pipe", "pipe"],
    encoding: "utf8",
    // The default limit is 1 MiB on each stream. Past it, Node.js stops
    // the child and cuts the output.
    maxBuffer: Infinity,
  });
  if (result.error) {
    throw result.error;
  }
  if (result.status === null) {
    throw new Error(`${file} was stopped by signal ${result.signal}`);
  }
  return [result.status, result.stdout, result.stderr];
}
