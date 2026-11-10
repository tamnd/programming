// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

using System;
using System.Globalization;

/// <summary>Parses a signed 64-bit integer with a strict syntax.</summary>
public static class IntegerParser
{
    /// <summary>
    /// Parses s as a signed 64-bit integer in the given radix, from 2 to 36. Throws a
    /// FormatException when s is not an optional sign and ASCII digits of the radix, and an
    /// OverflowException when the value does not fit in a long.
    /// </summary>
    public static long ParseInteger(string s, int radix)
    {
        if (radix == 10)
        {
            // AllowLeadingSign alone turns off the white space that the default style allows.
            return long.Parse(s, NumberStyles.AllowLeadingSign, CultureInfo.InvariantCulture);
        }
        // Convert.ToInt64 takes only the bases 2, 8, 10 and 16, accepts a 0x prefix and
        // reads a hex value above long.MaxValue as negative, so check the digits here.
        bool negative = s.StartsWith('-');
        int start = negative || s.StartsWith('+') ? 1 : 0;
        if (start == s.Length)
        {
            throw Invalid(s, radix);
        }
        foreach (char c in s.AsSpan(start))
        {
            int d = DigitValue(c);
            if (d < 0 || d >= radix)
            {
                throw Invalid(s, radix);
            }
        }
        long value = 0;
        foreach (char c in s.AsSpan(start))
        {
            // Add toward the sign, so long.MinValue fits. checked throws OverflowException.
            int d = DigitValue(c);
            value = checked(value * radix + (negative ? -d : d));
        }
        return value;
    }

    private static int DigitValue(char c) => c switch
    {
        >= '0' and <= '9' => c - '0',
        >= 'a' and <= 'z' => c - 'a' + 10,
        >= 'A' and <= 'Z' => c - 'A' + 10,
        _ => -1,
    };

    private static FormatException Invalid(string s, int radix) =>
        new($"invalid integer for radix {radix}: \"{s}\"");
}
