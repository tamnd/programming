// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

import java.time.LocalDate;

public final class AddDate {
    private AddDate() {}

    /**
     * Adds years and months at once, then days, and returns the date as YYYY-MM-DD.
     * plusMonths uses the last day of the new month when the day is not valid in it.
     */
    public static String addToDate(int year, int month, int day, int years, int months, int days) {
        return LocalDate.of(year, month, day)
                .plusMonths(years * 12L + months)
                .plusDays(days)
                .toString();
    }
}
