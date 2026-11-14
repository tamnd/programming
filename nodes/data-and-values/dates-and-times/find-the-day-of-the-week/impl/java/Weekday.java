// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

import java.time.LocalDate;
import java.time.temporal.IsoFields;

/** Finds the day of the week and the ISO week of a date. */
public final class Weekday {
    private Weekday() {}

    /** Returns the ISO 8601 day of the week: 1 for Monday to 7 for Sunday. */
    public static int dayOfWeek(int year, int month, int day) {
        return LocalDate.of(year, month, day).getDayOfWeek().getValue();
    }

    /** Returns the ISO 8601 week-based year and week number of a date. */
    public static int[] isoWeek(int year, int month, int day) {
        LocalDate date = LocalDate.of(year, month, day);
        return new int[] {date.get(IsoFields.WEEK_BASED_YEAR), date.get(IsoFields.WEEK_OF_WEEK_BASED_YEAR)};
    }
}
