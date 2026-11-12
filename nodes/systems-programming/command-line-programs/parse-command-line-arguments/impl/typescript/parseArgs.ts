// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

import { parseArgs as parse } from "node:util";

/** A usage error of the command line. */
export class UsageError extends Error {}

export interface Args {
  verbose: boolean;
  limit: number;
  files: string[];
}

const options = {
  verbose: { type: "boolean", short: "v", default: false },
  limit: { type: "string", short: "n", default: "10" },
} as const;

/**
 * Parses the arguments of "count [-v] [-n N] [--] [FILE...]" without the
 * program name. Throws UsageError for an unknown option, a missing value or
 * a value that is not a decimal number.
 */
export function parseArgs(args: string[]): Args {
  let parsed;
  try {
    parsed = parse({ args, options, strict: true, allowPositionals: true });
  } catch (err) {
    throw new UsageError((err as Error).message, { cause: err });
  }
  const { values, positionals } = parsed;
  // parseArgs has no number type, so the code converts the string.
  if (!/^[0-9]+$/.test(values.limit)) {
    throw new UsageError(`option --limit needs a number, not '${values.limit}'`);
  }
  return { verbose: values.verbose, limit: Number(values.limit), files: positionals };
}
