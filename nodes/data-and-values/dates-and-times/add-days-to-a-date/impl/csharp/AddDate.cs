// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

using System.Globalization;

public static class AddDate
{
    /// <summary>
    /// Adds years and months at once, then days, and returns the date as YYYY-MM-DD.
    /// AddMonths uses the last day of the new month when the day is not valid in it.
    /// </summary>
    public static string AddToDate(int year, int month, int day, int years, int months, int days) =>
        new DateOnly(year, month, day)
            .AddMonths(years * 12 + months)
            .AddDays(days)
            .ToString("yyyy'-'MM'-'dd", CultureInfo.InvariantCulture);
}
