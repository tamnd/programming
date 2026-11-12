// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

using System.Globalization;
using System.Text;

public static class NumberLine
{
    /// <summary>Returns the numbers 1 to n, separated by commas.</summary>
    public static string Build(int n)
    {
        // A StringBuilder appends to its buffer, so the loop does not copy
        // the whole result on each step.
        var sb = new StringBuilder();
        for (int i = 1; i <= n; i++)
        {
            if (i > 1)
            {
                sb.Append(',');
            }
            sb.Append(i.ToString(CultureInfo.InvariantCulture));
        }
        return sb.ToString();
    }
}
