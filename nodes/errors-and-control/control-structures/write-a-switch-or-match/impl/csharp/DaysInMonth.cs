// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

public static class DaysInMonth
{
    // Returns the number of days in a month of a common year, or 0 for a bad month.
    public static int Of(int month) => month switch
    {
        2 => 28,
        4 or 6 or 9 or 11 => 30,
        1 or 3 or 5 or 7 or 8 or 10 or 12 => 31,
        _ => 0,
    };
}
