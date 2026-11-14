// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

using System.Text;

/// <summary>Sorts the lines of a text file.</summary>
public static class SortedLines
{
    /// <summary>
    /// Returns the lines of the UTF-8 file at path, sorted by code point. A line ends at LF, and
    /// the LF is not part of it. A last line with no LF is a line too.
    /// </summary>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    public static List<string> Read(string path)
    {
        byte[] data = File.ReadAllBytes(path);
        var lines = new List<ReadOnlyMemory<byte>>();
        int start = 0;
        while (start < data.Length)
        {
            int end = Array.IndexOf(data, (byte)'\n', start);
            if (end < 0)
            {
                end = data.Length;
            }
            lines.Add(data.AsMemory(start, end - start));
            start = end + 1;
        }
        // string.CompareOrdinal compares UTF-16 chars, which is not the order of
        // the code points. SequenceCompareTo compares the UTF-8 bytes, which is.
        lines.Sort((a, b) => a.Span.SequenceCompareTo(b.Span));
        return lines.ConvertAll(line => Encoding.UTF8.GetString(line.Span));
    }
}
