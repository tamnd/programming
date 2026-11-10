// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

/** Returns the pattern of a date YYYY<sep>MM<sep>DD with no digit next to it. */
export function datePattern(sep: string): RegExp {
  // RegExp.escape makes each special character of sep match only itself.
  // The flag g finds all matches. The flag u reads the pattern as code points.
  const s = RegExp.escape(sep);
  return new RegExp(`(?<![0-9])(?<year>[0-9]{4})${s}(?<month>[0-9]{2})${s}(?<day>[0-9]{2})(?![0-9])`, "gu");
}

/** Changes each date YYYY<fromSep>MM<fromSep>DD in text to DD<toSep>MM<toSep>YYYY. */
export function reformatDates(text: string, fromSep: string, toSep: string): string {
  // In a replacement string, $ starts a pattern such as $1, $& or $<name>, and $$ is one $.
  const sep = toSep.replaceAll("$", "$$$$");
  return text.replaceAll(datePattern(fromSep), `$<day>${sep}$<month>${sep}$<year>`);
}

/** Returns [year, month, day] for each date YYYY<sep>MM<sep>DD in text. */
export function findDates(text: string, sep: string): string[][] {
  return Array.from(text.matchAll(datePattern(sep)), (m) => {
    const { year, month, day } = m.groups as { year: string; month: string; day: string };
    return [year, month, day];
  });
}
