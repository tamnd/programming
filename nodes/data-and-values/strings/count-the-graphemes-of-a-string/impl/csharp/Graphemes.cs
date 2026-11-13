// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

using System.Globalization;

public static class Graphemes
{
    // Count returns the number of extended grapheme clusters in s. A text
    // element of StringInfo is a grapheme. s.Length counts UTF-16 code units.
    public static int Count(string s)
    {
        return new StringInfo(s).LengthInTextElements;
    }
}
