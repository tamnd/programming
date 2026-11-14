// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

/**
 * Returns the seconds since 1970-01-01T00:00:00Z of a local time with the given UTC offset.
 * Date.UTC is not used, because it reads the years 0 to 99 as 1900 to 1999.
 */
export function unixTimestamp(
  year: number,
  month: number,
  day: number,
  hour: number,
  minute: number,
  second: number,
  offsetMinutes: number,
): number {
  const date = new Date(0);
  date.setUTCFullYear(year, month - 1, day);
  date.setUTCHours(hour, minute - offsetMinutes, second, 0);
  return date.getTime() / 1000;
}

/** Returns the current Unix time in whole seconds, rounded toward the past. */
export function nowUnixSeconds(): number {
  return Math.floor(Date.now() / 1000);
}

/** Returns the current Unix time in milliseconds. */
export function nowUnixMillis(): number {
  return Date.now();
}
