// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

public static class MakeSymlink
{
    /// <summary>
    /// Makes a symbolic link at link that holds target, and returns the
    /// target that the link holds, read back with LinkTarget. target is
    /// stored as it is: it need not exist, and a relative target is read
    /// from the directory of link. File.CreateSymbolicLink takes the new
    /// link first and the target second, the other order from ln -s. When
    /// link exists, also as a dangling link, it throws IOException, and when
    /// the directory of link does not exist, it throws
    /// DirectoryNotFoundException.
    /// </summary>
    public static string Make(string target, string link)
    {
        var info = File.CreateSymbolicLink(link, target);
        return info.LinkTarget ?? throw new IOException($"not a symbolic link: {link}");
    }
}
