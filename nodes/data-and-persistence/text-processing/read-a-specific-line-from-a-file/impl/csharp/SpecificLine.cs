// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

using System.Text;

/// <summary>Reads one line of a text file by its number.</summary>
public static class SpecificLine
{
    /// <summary>
    /// Returns line number of the UTF-8 file at path, or null if there is no such line. Line 1 is
    /// the first line. A line ends at LF, and the LF is not part of it. A last line with no LF is
    /// a line too. The method reads the file only up to the line.
    /// </summary>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    public static string? Read(string path, int number)
    {
        // The method reads bytes, not StreamReader.ReadLine, because ReadLine
        // also ends a line at a CR.
        using var stream = new BufferedStream(File.OpenRead(path));
        if (number < 1)
        {
            return null;
        }
        var line = new MemoryStream();
        int n = 1;
        int b;
        while ((b = stream.ReadByte()) != -1)
        {
            if (b != '\n')
            {
                if (n == number)
                {
                    line.WriteByte((byte)b);
                }
            }
            else if (n == number)
            {
                return Encoding.UTF8.GetString(line.ToArray());
            }
            else
            {
                n++;
            }
        }
        // A last line with no LF ends at the end of the file.
        return n == number && line.Length > 0 ? Encoding.UTF8.GetString(line.ToArray()) : null;
    }
}
