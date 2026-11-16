// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

using System.Globalization;

/// <summary>A number with a fixed count of decimals.</summary>
public static class Fixed
{
    /// <summary>
    /// Returns x with exactly digits digits after the decimal point. The
    /// format F rounds the exact binary value of x, and the invariant culture
    /// gives a dot.
    /// </summary>
    public static string Format(double x, int digits) =>
        x.ToString("F" + digits.ToString(CultureInfo.InvariantCulture), CultureInfo.InvariantCulture);
}
