// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

/** Returns the number of days in a month (1 to 12) of a year. */
export function daysInMonth(year, month) {
  const date = new Date(0);
  // Day 0 of the next month is the last day of this month.
  date.setUTCFullYear(year, month, 0);
  return date.getUTCDate();
}

/**
 * Adds years and months at once, then days, and returns the date as YYYY-MM-DD.
 * A day after the end of the new month becomes its last day.
 * The code uses the UTC methods of Date, so the local time zone has no effect.
 */
export function addToDate(year, month, day, years, months, days) {
  const total = year * 12 + (month - 1) + years * 12 + months;
  const newYear = Math.floor(total / 12);
  const newMonth = total - newYear * 12 + 1;
  const newDay = Math.min(day, daysInMonth(newYear, newMonth));
  const date = new Date(0);
  // setUTCFullYear accepts a day after the end of the month and moves to the next months.
  date.setUTCFullYear(newYear, newMonth - 1, newDay + days);
  return date.toISOString().slice(0, 10);
}
