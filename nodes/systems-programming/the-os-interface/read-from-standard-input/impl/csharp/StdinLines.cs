// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

/// <summary>Reads the lines of standard input.</summary>
public static class StdinLines
{
    /// <summary>Reads the reader to its end and returns its lines, each without "\n" or "\r\n".</summary>
    public static List<string> ReadLines(TextReader reader)
    {
        var lines = new List<string>();
        // ReadLine returns null at the end of the input.
        while (reader.ReadLine() is string line)
        {
            lines.Add(line);
        }
        return lines;
    }

    /// <summary>Reads standard input to its end and returns its lines.</summary>
    public static List<string> ReadStdinLines() => ReadLines(Console.In);
}
