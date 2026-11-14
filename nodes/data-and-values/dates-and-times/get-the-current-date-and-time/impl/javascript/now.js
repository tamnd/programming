// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

/**
 * Returns the UTC date and time of epochNs as "YYYY-MM-DDTHH:MM:SS.mmmZ".
 * @param {bigint} epochNs nanoseconds since 1970-01-01T00:00:00Z
 * @returns {string}
 */
export function utcTimestamp(epochNs) {
  // bigint division truncates toward zero. Step back one millisecond for a
  // negative reading with a rest, so the result goes toward the past.
  let ms = epochNs / 1_000_000n;
  if (epochNs % 1_000_000n < 0n) ms -= 1n;
  // A Date holds whole milliseconds, and toISOString always writes three digits.
  return new Date(Number(ms)).toISOString();
}

/** The real clock: Date.now() in nanoseconds. It has whole milliseconds only. */
export function systemNs() {
  return BigInt(Date.now()) * 1_000_000n;
}

/**
 * Reads the clock once and returns the UTC date and time of the reading.
 * @param {() => bigint} clock
 * @returns {string}
 */
export function nowUtc(clock = systemNs) {
  return utcTimestamp(clock());
}
