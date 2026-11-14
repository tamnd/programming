// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

/// <summary>
/// The lines of a file. Newlines is the number of LF bytes, the number that wc -l prints.
/// Lines also counts a last line that has no LF.
/// </summary>
public readonly record struct LineCount(long Newlines, long Lines)
{
    /// <summary>Counts the lines of the file at path. It reads the bytes and does not decode them.</summary>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    public static LineCount Read(string path)
    {
        var buffer = new byte[64 * 1024];
        long newlines = 0;
        byte last = (byte)'\n'; // an empty file has no line without an LF
        using FileStream stream = File.OpenRead(path);
        int n;
        // Read returns 0 at the end of the file.
        while ((n = stream.Read(buffer)) > 0)
        {
            newlines += buffer.AsSpan(0, n).Count((byte)'\n');
            last = buffer[n - 1];
        }
        return new LineCount(newlines, newlines + (last != (byte)'\n' ? 1 : 0));
    }
}
