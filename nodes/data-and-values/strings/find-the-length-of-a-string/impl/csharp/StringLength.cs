// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

using System.Text;

/// <summary>Finds the lengths of a string in UTF-8 bytes, in UTF-16 code units and in code points.</summary>
public static class StringLength
{
    /// <summary>Returns the length of s in UTF-8 bytes, in UTF-16 code units and in code points.</summary>
    public static int[] Lengths(string s)
    {
        // A string is a sequence of UTF-16 char values, and Length counts them.
        // A Rune is one Unicode scalar value, that is, one code point.
        var codePoints = 0;
        foreach (var _ in s.EnumerateRunes())
        {
            codePoints++;
        }
        return [Encoding.UTF8.GetByteCount(s), s.Length, codePoints];
    }
}
