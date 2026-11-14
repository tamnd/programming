// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

/// <summary>Removes lines from a file and writes the rest back under the same name.</summary>
public static class RemoveLines
{
    /// <summary>
    /// Removes count lines from the file at path, from line start, and returns how many it
    /// removed. Line 1 is the first line. A line ends at LF, and the LF goes with the line. The
    /// result is less than count if the file ends first. If no line goes, the file is not written.
    /// Otherwise the rest of the file goes to a new file in the same directory, and File.Move puts
    /// it in the place of the old one.
    /// </summary>
    /// <exception cref="ArgumentOutOfRangeException">start is less than 1 or count is less than 0.</exception>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    public static int Remove(string path, int start, int count)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(start, 1);
        ArgumentOutOfRangeException.ThrowIfNegative(count);
        // Bytes, not lines of text: the bytes that stay do not change.
        byte[] data = File.ReadAllBytes(path);
        int offset = 0;
        for (int n = 1; n < start && offset < data.Length; n++)
        {
            offset = NextLine(data, offset);
        }
        int from = offset;
        int removed = 0;
        while (removed < count && offset < data.Length)
        {
            offset = NextLine(data, offset);
            removed++;
        }
        if (removed == 0)
        {
            return 0;
        }
        string dir = Path.GetDirectoryName(Path.GetFullPath(path))!;
        string tmp = Path.Combine(dir, $".{Path.GetFileName(path)}.{Guid.NewGuid():N}.tmp");
        try
        {
            // CreateNew fails if a file with the name exists.
            using (var stream = new FileStream(tmp, FileMode.CreateNew, FileAccess.Write))
            {
                stream.Write(data, 0, from);
                stream.Write(data, offset, data.Length - offset);
                // Flush(true) also writes the data from the operating system to the disk.
                stream.Flush(true);
            }
            if (!OperatingSystem.IsWindows())
            {
                File.SetUnixFileMode(tmp, File.GetUnixFileMode(path));
            }
            File.Move(tmp, path, overwrite: true);
        }
        catch
        {
            File.Delete(tmp);
            throw;
        }
        return removed;
    }

    // Returns the offset just after the line that starts at offset.
    private static int NextLine(byte[] data, int offset)
    {
        int lf = Array.IndexOf(data, (byte)'\n', offset);
        return lf == -1 ? data.Length : lf + 1;
    }
}
