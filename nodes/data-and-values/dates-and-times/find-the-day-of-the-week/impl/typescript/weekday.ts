// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

const MS_PER_DAY = 24 * 60 * 60 * 1000;

/**
 * Returns a Date at midnight UTC of a date.
 * setUTCFullYear takes the year as it is. Date.UTC reads the years 0 to 99 as 1900 to 1999.
 */
function utcDate(year: number, month: number, day: number): Date {
  const date = new Date(0);
  date.setUTCFullYear(year, month - 1, day);
  return date;
}

/**
 * Returns the ISO 8601 day of the week: 1 for Monday to 7 for Sunday.
 * getUTCDay returns 0 for Sunday, so 0 becomes 7.
 */
export function dayOfWeek(year: number, month: number, day: number): number {
  return utcDate(year, month, day).getUTCDay() || 7;
}

/**
 * Returns the ISO 8601 week-based year and week number of a date.
 * The Thursday of the same week is always in the week-based year.
 */
export function isoWeek(year: number, month: number, day: number): [number, number] {
  const date = utcDate(year, month, day);
  const thursday = new Date(date.getTime() + (4 - (date.getUTCDay() || 7)) * MS_PER_DAY);
  const weekYear = thursday.getUTCFullYear();
  const january1 = utcDate(weekYear, 1, 1);
  const week = Math.floor((thursday.getTime() - january1.getTime()) / MS_PER_DAY / 7) + 1;
  return [weekYear, week];
}
