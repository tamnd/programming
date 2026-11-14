// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

using System.Text;

/// <summary>Gets the first lines and the last lines of a text file.</summary>
public static class FirstAndLastLines
{
    /// <summary>
    /// Returns the first count lines and the last count lines of the UTF-8 file at path. If the
    /// file has fewer than count lines, each list has all the lines. A line ends at LF, and the
    /// LF is not part of it. A last line with no LF is a line too.
    /// </summary>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    public static (List<string> First, List<string> Last) Read(string path, int count)
    {
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
            lines.Add(Encoding.UTF8.GetString(data, start, end - start));
            start = end + 1;
        }
        int take = Math.Min(count, lines.Count);
        // GetRange takes a start index and a number of items, not an end index.
        return (lines.GetRange(0, take), lines.GetRange(lines.Count - take, take));
    }
}
