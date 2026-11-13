// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

public static class CopyFile
{
    /// <summary>
    /// Copies the bytes and the permission bits of the file at src to dst, and
    /// replaces a file at dst. File.Copy with overwrite true does the copy. A
    /// directory at src or dst gives an IOException. On Unix, the code sets the
    /// permission bits of dst after the copy, so that they are also correct when
    /// the copy replaces a file. Each other error, for example
    /// FileNotFoundException, goes to the caller.
    /// </summary>
    public static void Copy(string src, string dst)
    {
        if (Directory.Exists(src))
        {
            throw new IOException($"Is a directory: '{src}'");
        }
        if (Directory.Exists(dst))
        {
            throw new IOException($"Is a directory: '{dst}'");
        }
        File.Copy(src, dst, overwrite: true);
        if (!OperatingSystem.IsWindows())
        {
            File.SetUnixFileMode(dst, File.GetUnixFileMode(src));
        }
    }
}
