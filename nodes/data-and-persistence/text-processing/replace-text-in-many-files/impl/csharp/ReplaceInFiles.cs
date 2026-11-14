// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

using System.Text;

/// <summary>Replaces a text with another text in each file of a list.</summary>
public static class ReplaceInFiles
{
    /// <summary>
    /// Replaces each instance of oldText with newText in each file, and returns the number of
    /// instances in each file. oldText and newText are plain text, not patterns. The code compares
    /// the UTF-8 bytes of oldText with the bytes of each file, so an instance can contain LF bytes.
    /// Instances do not overlap. A file with no instance is not written. A changed file goes to a
    /// new file in the same directory, and File.Move puts it in the place of the old one. The files
    /// are done in order, and the first error stops the method: the files before have the new
    /// text, and the files after do not change.
    /// </summary>
    /// <exception cref="ArgumentException">oldText is empty.</exception>
    /// <exception cref="FileNotFoundException">A file does not exist.</exception>
    public static int[] Replace(IReadOnlyList<string> paths, string oldText, string newText)
    {
        ArgumentException.ThrowIfNullOrEmpty(oldText);
        byte[] oldBytes = Encoding.UTF8.GetBytes(oldText);
        byte[] newBytes = Encoding.UTF8.GetBytes(newText);
        int[] counts = new int[paths.Count];
        for (int i = 0; i < paths.Count; i++)
        {
            byte[] data = File.ReadAllBytes(paths[i]);
            var output = new MemoryStream(data.Length);
            int from = 0;
            int at;
            while ((at = data.AsSpan(from).IndexOf(oldBytes)) != -1)
            {
                output.Write(data, from, at);
                output.Write(newBytes);
                counts[i]++;
                // The search goes on after the end of the instance.
                from += at + oldBytes.Length;
            }
            if (counts[i] > 0)
            {
                output.Write(data, from, data.Length - from);
                WriteAtomically(paths[i], output.ToArray());
            }
        }
        return counts;
    }

    // Writes data to a new file next to path, and moves it over path.
    private static void WriteAtomically(string path, byte[] data)
    {
        string dir = Path.GetDirectoryName(Path.GetFullPath(path))!;
        string tmp = Path.Combine(dir, $".{Path.GetFileName(path)}.{Guid.NewGuid():N}.tmp");
        try
        {
            // CreateNew fails if a file with the name exists.
            using (var stream = new FileStream(tmp, FileMode.CreateNew, FileAccess.Write))
            {
                stream.Write(data);
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
    }
}
