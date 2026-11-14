// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

const MS_PER_DAY = 24 * 60 * 60 * 1000;

/**
 * Returns the milliseconds from the epoch to midnight UTC of a date.
 * setUTCFullYear takes the year as it is. Date.UTC reads the years 0 to 99 as 1900 to 1999.
 */
function utcMidnight(year, month, day) {
  const date = new Date(0);
  date.setUTCFullYear(year, month - 1, day);
  return date.getTime();
}

/**
 * Returns the number of days from the start date to the end date.
 * In UTC each day has exactly 86 400 000 ms, so the division is exact.
 */
export function daysBetween(startYear, startMonth, startDay, endYear, endMonth, endDay) {
  return (utcMidnight(endYear, endMonth, endDay) - utcMidnight(startYear, startMonth, startDay)) / MS_PER_DAY;
}
