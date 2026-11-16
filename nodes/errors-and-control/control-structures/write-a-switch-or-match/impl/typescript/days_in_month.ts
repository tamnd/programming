// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

/** Returns the number of days in a month of a common year, or 0 for a bad month. */
export function daysInMonth(month: number): number {
  switch (month) {
    case 2:
      return 28;
    case 4:
    case 6:
    case 9:
    case 11:
      return 30;
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      return 31;
    default:
      return 0;
  }
}
