// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

/// <summary>Changes the case of the ASCII letters of a string.</summary>
public static class AsciiCase
{
    /// <summary>
    /// Returns s with a to z changed to A to Z. ToUpperInvariant changes the
    /// letters of Unicode too, so the function changes only the ASCII letters.
    /// </summary>
    public static string ToUpper(string s) =>
        string.Create(s.Length, s, static (span, src) =>
        {
            for (int i = 0; i < src.Length; i++)
            {
                char c = src[i];
                span[i] = c >= 'a' && c <= 'z' ? (char)(c - 'a' + 'A') : c;
            }
        });

    /// <summary>Returns s with A to Z changed to a to z.</summary>
    public static string ToLower(string s) =>
        string.Create(s.Length, s, static (span, src) =>
        {
            for (int i = 0; i < src.Length; i++)
            {
                char c = src[i];
                span[i] = c >= 'A' && c <= 'Z' ? (char)(c - 'A' + 'a') : c;
            }
        });
}
