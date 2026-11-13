// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

public static class TruncateFile
{
    /// <summary>
    /// Sets the size of the existing file at path to length bytes. FileMode.Open
    /// does not make a missing file, so a missing file gives FileNotFoundException.
    /// FileStream.SetLength cuts the bytes after length, or adds zero bytes up to
    /// length, and gives ArgumentOutOfRangeException for a length under 0. A
    /// directory at path gives an IOException.
    /// </summary>
    public static void Truncate(string path, long length)
    {
        ArgumentOutOfRangeException.ThrowIfNegative(length);
        if (Directory.Exists(path))
        {
            throw new IOException($"Is a directory: '{path}'");
        }
        using var stream = new FileStream(path, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
        stream.SetLength(length);
    }
}
