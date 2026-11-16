// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

using System.Globalization;

/// <summary>An integer with a separator between each group of 3 digits.</summary>
public static class Group
{
    /// <summary>
    /// Returns n in decimal with a comma between each group of 3 digits. The
    /// invariant culture gives the comma on each computer.
    /// </summary>
    public static string GroupThousands(long n) =>
        n.ToString("N0", CultureInfo.InvariantCulture);
}
