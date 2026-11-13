// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

public static class FileSize
{
    /// <summary>
    /// Returns the size in bytes of the regular file at path. FileInfo.Length
    /// gives FileNotFoundException for a missing file. For a directory it also
    /// gives FileNotFoundException, so the code throws an IOException first.
    /// For a symbolic link, FileInfo.Length is the size of the link itself, so
    /// the code gets the FileInfo of the final target first.
    /// </summary>
    public static long Size(string path)
    {
        if (Directory.Exists(path))
        {
            throw new IOException($"Is a directory: '{path}'");
        }
        var info = new FileInfo(path);
        if (info.LinkTarget != null)
        {
            info = (FileInfo?)info.ResolveLinkTarget(returnFinalTarget: true) ?? info;
        }
        return info.Length;
    }
}
