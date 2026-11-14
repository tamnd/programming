// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

import java.time.LocalDate;
import java.time.temporal.ChronoUnit;

/** Counts the days between two dates. */
public final class DateDiff {
    private DateDiff() {}

    /** Returns the number of days from the start date to the end date. */
    public static long daysBetween(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay) {
        LocalDate start = LocalDate.of(startYear, startMonth, startDay);
        LocalDate end = LocalDate.of(endYear, endMonth, endDay);
        return ChronoUnit.DAYS.between(start, end);
    }
}
