// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

import java.time.Year;

/** Checks for a leap year of the proleptic Gregorian calendar. */
public final class Leap {
    private Leap() {}

    /** Returns true for a leap year. Year.isLeap uses the ISO proleptic calendar with year 0. */
    public static boolean isLeapYear(int year) {
        return Year.isLeap(year);
    }
}
