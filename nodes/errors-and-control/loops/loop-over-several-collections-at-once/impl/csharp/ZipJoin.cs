// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

using System.Globalization;

public static class ZipJoin
{
    // Joins the items at each index, up to the end of the shortest list.
    public static List<string> Of(IEnumerable<string> lower, IEnumerable<string> upper, IEnumerable<int> digits)
    {
        var result = new List<string>();
        // Zip with three sequences stops at the end of the shortest one.
        foreach (var (a, b, d) in lower.Zip(upper, digits))
        {
            result.Add(a + b + d.ToString(CultureInfo.InvariantCulture));
        }
        return result;
    }
}
