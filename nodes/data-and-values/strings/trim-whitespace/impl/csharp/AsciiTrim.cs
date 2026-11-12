// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

/// <summary>Trims the ASCII white space from a string.</summary>
public static class AsciiTrim
{
    // Trim() with no argument removes each char of Char.IsWhiteSpace, which
    // includes U+00A0 and U+3000, so the class gives the 6 ASCII characters.
    private static readonly char[] Space = { ' ', '\t', '\n', '\v', '\f', '\r' };

    /// <summary>Returns s with the white space at the start and at the end removed.</summary>
    public static string Trim(string s) => s.Trim(Space);

    /// <summary>Returns s with the white space at the start removed.</summary>
    public static string TrimStart(string s) => s.TrimStart(Space);
}
