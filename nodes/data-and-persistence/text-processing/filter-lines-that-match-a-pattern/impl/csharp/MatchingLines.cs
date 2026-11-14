// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

using System.Text;
using System.Text.RegularExpressions;

/// <summary>Selects the lines of a text file that match a regular expression.</summary>
public static class MatchingLines
{
    /// <summary>
    /// Returns the lines of the UTF-8 file at path that match pattern. A line matches if the
    /// pattern matches some part of it. A line ends at LF, and the LF is not part of it. A last
    /// line with no LF is a line too.
    /// </summary>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    /// <exception cref="ArgumentException">The pattern is not valid.</exception>
    public static List<string> Read(string path, string pattern)
    {
        var regex = new Regex(pattern);
        byte[] data = File.ReadAllBytes(path);
        var lines = new List<string>();
        int start = 0;
        while (start < data.Length)
        {
            int end = Array.IndexOf(data, (byte)'\n', start);
            if (end < 0)
            {
                end = data.Length;
            }
            string line = Encoding.UTF8.GetString(data, start, end - start);
            // IsMatch looks for a match in some part of the line.
            if (regex.IsMatch(line))
            {
                lines.Add(line);
            }
            start = end + 1;
        }
        return lines;
    }
}
