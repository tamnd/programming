// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

/**
 * Returns the pattern of a date YYYY<sep>MM<sep>DD with no digit next to it.
 * @param {string} sep
 * @returns {RegExp}
 */
export function datePattern(sep) {
  // RegExp.escape makes each special character of sep match only itself.
  // The flag g finds all matches. The flag u reads the pattern as code points.
  const s = RegExp.escape(sep);
  return new RegExp(`(?<![0-9])(?<year>[0-9]{4})${s}(?<month>[0-9]{2})${s}(?<day>[0-9]{2})(?![0-9])`, "gu");
}

/**
 * Changes each date YYYY<fromSep>MM<fromSep>DD in text to DD<toSep>MM<toSep>YYYY.
 * @param {string} text
 * @param {string} fromSep
 * @param {string} toSep
 * @returns {string}
 */
export function reformatDates(text, fromSep, toSep) {
  // In a replacement string, $ starts a pattern such as $1, $& or $<name>, and $$ is one $.
  const sep = toSep.replaceAll("$", "$$$$");
  return text.replaceAll(datePattern(fromSep), `$<day>${sep}$<month>${sep}$<year>`);
}

/**
 * Returns [year, month, day] for each date YYYY<sep>MM<sep>DD in text.
 * @param {string} text
 * @param {string} sep
 * @returns {string[][]}
 */
export function findDates(text, sep) {
  return Array.from(text.matchAll(datePattern(sep)), (m) => [m.groups.year, m.groups.month, m.groups.day]);
}
