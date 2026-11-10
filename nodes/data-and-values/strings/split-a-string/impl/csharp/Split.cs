// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

using System.Text.RegularExpressions;

/// <summary>Splits strings on a literal separator, on white space and on a regular expression.</summary>
public static class Splitter
{
    /// <summary>
    /// Splits s on the literal separator sep and keeps the empty fields. With a limit n &gt; 0
    /// the result has at most n fields, and the last field holds the rest of s. A limit of 0
    /// means no limit.
    /// </summary>
    public static string[] Split(string s, string sep, int limit = 0) =>
        limit > 0 ? s.Split(sep, limit) : s.Split(sep);

    /// <summary>Splits s on runs of white space. The result has no empty fields.</summary>
    public static string[] SplitWhitespace(string s) =>
        s.Split((char[]?)null, StringSplitOptions.RemoveEmptyEntries);

    /// <summary>Splits s on each match of the regular expression pattern.</summary>
    public static string[] SplitRegex(string s, string pattern) => Regex.Split(s, pattern);
}
