// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

using System.Text.RegularExpressions;

/// <summary>Finds dates with a regular expression and writes them in another order.</summary>
public static class ReformatDates
{
    /// <summary>Returns the pattern of a date YYYY(sep)MM(sep)DD with no digit next to it.</summary>
    public static Regex DatePattern(string sep)
    {
        // Regex.Escape makes each special character of sep match only itself.
        // [0-9] is ASCII only. \d also matches other decimal digits, such as ٢.
        string s = Regex.Escape(sep);
        return new Regex($"(?<![0-9])(?<year>[0-9]{{4}}){s}(?<month>[0-9]{{2}}){s}(?<day>[0-9]{{2}})(?![0-9])");
    }

    /// <summary>Changes each date YYYY(fromSep)MM(fromSep)DD in text to DD(toSep)MM(toSep)YYYY.</summary>
    public static string Reformat(string text, string fromSep, string toSep)
    {
        // In a replacement string, $ starts a substitution such as $1 or ${name}, and $$ is one $.
        string sep = toSep.Replace("$", "$$");
        return DatePattern(fromSep).Replace(text, "${day}" + sep + "${month}" + sep + "${year}");
    }

    /// <summary>Returns [year, month, day] for each date YYYY(sep)MM(sep)DD in text.</summary>
    public static List<string[]> FindDates(string text, string sep) =>
        DatePattern(sep).Matches(text)
            .Select(m => new[] { m.Groups["year"].Value, m.Groups["month"].Value, m.Groups["day"].Value })
            .ToList();
}
