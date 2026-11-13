// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

public static class DeleteFile
{
    /// <summary>
    /// Deletes the file at path. Gives true if a file was there and false if not.
    /// File.Delete gives no error and no result for a missing file, so the code
    /// looks at the entry first. File.GetAttributes does not follow a symbolic
    /// link, so a link is deleted, not its target. A directory is an error.
    /// Each other error, for example UnauthorizedAccessException, goes to the caller.
    /// </summary>
    public static bool Delete(string path)
    {
        FileAttributes attrs;
        try
        {
            attrs = File.GetAttributes(path);
        }
        catch (FileNotFoundException)
        {
            return false;
        }
        catch (DirectoryNotFoundException)
        {
            return false; // a missing directory, or a path through a regular file
        }
        if (attrs.HasFlag(FileAttributes.Directory) && !attrs.HasFlag(FileAttributes.ReparsePoint))
        {
            throw new IOException($"{path} is a directory");
        }
        File.Delete(path);
        return true;
    }
}
