// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

/**
 * Returns the seconds since 1970-01-01T00:00:00Z of a local time with the given UTC offset.
 * Date.UTC is not used, because it reads the years 0 to 99 as 1900 to 1999.
 * @param {number} year
 * @param {number} month 1 to 12
 * @param {number} day
 * @param {number} hour
 * @param {number} minute
 * @param {number} second
 * @param {number} offsetMinutes positive east of Greenwich
 * @returns {number}
 */
export function unixTimestamp(year, month, day, hour, minute, second, offsetMinutes) {
  const date = new Date(0);
  date.setUTCFullYear(year, month - 1, day);
  date.setUTCHours(hour, minute - offsetMinutes, second, 0);
  return date.getTime() / 1000;
}

/** Returns the current Unix time in whole seconds, rounded toward the past. */
export function nowUnixSeconds() {
  return Math.floor(Date.now() / 1000);
}

/** Returns the current Unix time in milliseconds. */
export function nowUnixMillis() {
  return Date.now();
}
