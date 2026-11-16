// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

using System.Globalization;

/// <summary>One line of a table with fixed columns.</summary>
public static class Row
{
    /// <summary>
    /// Returns name in 10 columns on the left, qty in 5 on the right and price
    /// with 2 decimals in 8 on the right. The invariant culture keeps the
    /// decimal point a dot. A width is a minimum, so a long value is not cut.
    /// </summary>
    public static string FormatRow(string name, long qty, double price) =>
        string.Format(CultureInfo.InvariantCulture, "{0,-10}|{1,5}|{2,8:F2}", name, qty, price);
}
