// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

public static class FileMtime
{
    /// <summary>
    /// Returns the modification time of the file or directory at path, in whole
    /// seconds since 1970-01-01T00:00:00Z. File.GetLastWriteTimeUtc gives no
    /// error for a missing file: it gives 1601-01-01T00:00:00Z. So the code
    /// throws FileNotFoundException first. ToUnixTimeSeconds gives the floor,
    /// also before 1970. For a symbolic link, File.GetLastWriteTimeUtc gives the
    /// time of the link itself, so the code gets the final target first.
    /// </summary>
    public static long Mtime(string path)
    {
        if (!File.Exists(path) && !Directory.Exists(path))
        {
            throw new FileNotFoundException($"No such file: '{path}'", path);
        }
        FileSystemInfo info = Directory.Exists(path) ? new DirectoryInfo(path) : new FileInfo(path);
        if (info.LinkTarget != null)
        {
            info = info.ResolveLinkTarget(returnFinalTarget: true) ?? info;
        }
        return new DateTimeOffset(info.LastWriteTimeUtc).ToUnixTimeSeconds();
    }
}
