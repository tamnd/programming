// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

using System.Text;

public enum WriteMode
{
    Truncate,
    Append,
    Atomic,
}

public static class WriteFile
{
    // Encoding.UTF8 writes a byte order mark. This encoding writes none, and
    // it throws on a string that is not valid UTF-16.
    private static readonly UTF8Encoding Utf8NoBom = new(false, true);

    // Writes text to path as UTF-8, with no change to the line ends.
    public static void WriteText(string path, string text, WriteMode mode)
    {
        switch (mode)
        {
            case WriteMode.Truncate:
                File.WriteAllText(path, text, Utf8NoBom);
                break;
            case WriteMode.Append:
                File.AppendAllText(path, text, Utf8NoBom);
                break;
            case WriteMode.Atomic:
                Replace(path, text);
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(mode));
        }
    }

    private static void Replace(string path, string text)
    {
        // The temporary file is in the same directory, so the move does not
        // cross a file system. FileMode.CreateNew fails if the name exists.
        string full = Path.GetFullPath(path);
        string tmp = Path.Combine(Path.GetDirectoryName(full)!, $".{Path.GetFileName(full)}.{Guid.NewGuid():N}.tmp");
        var stream = new FileStream(tmp, FileMode.CreateNew, FileAccess.Write);
        try
        {
            using (stream)
            {
                stream.Write(Utf8NoBom.GetBytes(text));
                stream.Flush(flushToDisk: true);
            }
            File.Move(tmp, full, overwrite: true);
        }
        catch
        {
            File.Delete(tmp);
            throw;
        }
    }
}
