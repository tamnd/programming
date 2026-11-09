// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

using System.Text;

/// <summary>The number of lines of a text file and the number of code points in its longest line.</summary>
public readonly record struct LineStats(long Lines, long Longest)
{
    // throwOnInvalidBytes: true makes the decoder throw on a bad byte sequence.
    // The default UTF-8 decoder puts U+FFFD in its place and does not tell you.
    private static readonly UTF8Encoding StrictUtf8 = new(encoderShouldEmitUTF8Identifier: false, throwOnInvalidBytes: true);

    /// <summary>Reads a UTF-8 text file line by line.</summary>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    /// <exception cref="DecoderFallbackException">The bytes are not valid UTF-8.</exception>
    public static LineStats Read(string path)
    {
        long lines = 0, longest = 0;
        foreach (string line in File.ReadLines(path, StrictUtf8))
        {
            lines++;
            longest = Math.Max(longest, line.EnumerateRunes().Count());
        }
        return new LineStats(lines, longest);
    }
}
