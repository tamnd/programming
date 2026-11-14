// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

using System.Text;

/// <summary>Removes the duplicate lines of a text file.</summary>
public static class UniqueLines
{
    /// <summary>
    /// Returns the lines of the UTF-8 file at path, with only the first copy of each line, in the
    /// order of the file. A line ends at LF, and the LF is not part of it. A last line with no LF
    /// is a line too.
    /// </summary>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    public static List<string> Read(string path)
    {
        byte[] data = File.ReadAllBytes(path);
        var seen = new HashSet<string>(StringComparer.Ordinal);
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
            // Add returns false if the set has the line already.
            if (seen.Add(line))
            {
                lines.Add(line);
            }
            start = end + 1;
        }
        return lines;
    }
}
