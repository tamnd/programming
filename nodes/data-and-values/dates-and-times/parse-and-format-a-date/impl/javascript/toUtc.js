// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

// The date-time of RFC 3339 section 5.6, with a fraction of 1 to 3 digits.
// Date accepts more than this, and it rolls an invalid day over into the next
// month, so the pattern checks the syntax and daysInMonth checks the day.
const RFC3339 =
  /^(\d{4})-(\d{2})-(\d{2})[Tt]([01]\d|2[0-3]):[0-5]\d:[0-5]\d(\.\d{1,3})?([Zz]|[+-]([01]\d|2[0-3]):[0-5]\d)$/;

function daysInMonth(year, month) {
  const leap = year % 4 === 0 && (year % 100 !== 0 || year % 400 === 0);
  return [31, leap ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31][month - 1];
}

// Returns the instant of an RFC 3339 timestamp in UTC, as YYYY-MM-DDTHH:MM:SS.sssZ.
// Throws RangeError when timestamp is not an RFC 3339 date-time with an offset,
// or when its date is not in the calendar.
export function toUtc(timestamp) {
  const match = RFC3339.exec(timestamp);
  if (match === null) {
    throw new RangeError(`not an RFC 3339 date-time: ${JSON.stringify(timestamp)}`);
  }
  const year = Number(match[1]);
  const month = Number(match[2]);
  const day = Number(match[3]);
  if (month < 1 || month > 12 || day < 1 || day > daysInMonth(year, month)) {
    throw new RangeError(`not a date in the calendar: ${JSON.stringify(timestamp)}`);
  }
  // The pattern makes sure that there is an offset, so the time zone of the
  // machine has no effect.
  return new Date(timestamp.toUpperCase()).toISOString();
}
