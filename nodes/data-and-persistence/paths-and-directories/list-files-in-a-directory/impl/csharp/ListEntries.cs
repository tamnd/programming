// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

/// <summary>Lists the entries of a directory, or of a whole tree.</summary>
public static class ListEntries
{
    private static readonly EnumerationOptions Options = new()
    {
        // The default skips Hidden and System. On Unix, a name that starts with "." is Hidden.
        AttributesToSkip = 0,
        // The default ignores the directories that it cannot open.
        IgnoreInaccessible = false,
        // RecurseSubdirectories follows a symbolic link to a directory, so the code recurses itself.
        RecurseSubdirectories = false,
    };

    /// <summary>
    /// Returns the relative paths of the entries under root, sorted by code point. A directory has
    /// a "/" at the end. A symbolic link is listed, but it is never followed.
    /// </summary>
    /// <exception cref="DirectoryNotFoundException">root does not exist.</exception>
    /// <exception cref="IOException">root is a file.</exception>
    public static List<string> List(string root, bool recursive)
    {
        if (File.Exists(root))
        {
            throw new IOException($"{root}: not a directory");
        }
        var output = new List<string>();
        Walk(new DirectoryInfo(root), "", recursive, output);
        // Ordinal order compares UTF-16 code units. That is code point order
        // for names with no characters outside the Basic Multilingual Plane.
        output.Sort(StringComparer.Ordinal);
        return output;
    }

    private static void Walk(DirectoryInfo dir, string prefix, bool recursive, List<string> output)
    {
        foreach (FileSystemInfo info in dir.EnumerateFileSystemInfos("*", Options))
        {
            // LinkTarget is null for an entry that is not a symbolic link.
            if (info is DirectoryInfo sub && info.LinkTarget is null)
            {
                output.Add(prefix + info.Name + "/");
                if (recursive)
                {
                    Walk(sub, prefix + info.Name + "/", recursive, output);
                }
            }
            else
            {
                output.Add(prefix + info.Name);
            }
        }
    }
}
