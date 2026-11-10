// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

/**
 * Splits s on the literal separator sep and keeps the empty fields. With a
 * limit n > 0 the result has at most n fields, and the last field holds the
 * rest of s. A limit of 0 means no limit.
 */
export function split(s: string, sep: string, limit = 0): string[] {
  const fields = s.split(sep);
  if (limit === 0 || fields.length <= limit) {
    return fields;
  }
  // The limit argument of String.prototype.split drops the rest, so join it.
  return [...fields.slice(0, limit - 1), fields.slice(limit - 1).join(sep)];
}

/** Splits s on runs of white space. The result has no empty fields. */
export function splitWhitespace(s: string): string[] {
  return s.split(/\s+/).filter((field) => field !== "");
}

/** Splits s on each match of the regular expression pattern. */
export function splitRegex(s: string, pattern: string): string[] {
  return s.split(new RegExp(pattern, "u"));
}
