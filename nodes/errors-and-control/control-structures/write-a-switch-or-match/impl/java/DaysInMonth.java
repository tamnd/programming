// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

final class DaysInMonth {
  private DaysInMonth() {}

  /** Returns the number of days in a month of a common year, or 0 for a bad month. */
  static int daysInMonth(int month) {
    // A switch expression: each case -> runs only its own code.
    return switch (month) {
      case 2 -> 28;
      case 4, 6, 9, 11 -> 30;
      case 1, 3, 5, 7, 8, 10, 12 -> 31;
      default -> 0;
    };
  }
}
