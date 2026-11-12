// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

using System.Diagnostics;
using System.Text;

public static class AppendLine
{
    private static readonly UTF8Encoding Utf8 = new(false, true);

    /// <summary>Adds line and one "\n" to the end of the file, in one write call.</summary>
    /// <remarks>
    /// FileMode.Append moves to the end of the file only when the file opens, and .NET has
    /// no option for O_APPEND. So each write holds the file with FileShare.None, and a
    /// second writer waits until the first closes the file.
    /// </remarks>
    public static void Append(string path, string line)
    {
        if (line.Contains('\n') || line.Contains('\r'))
        {
            throw new ArgumentException("the line must not hold \\n or \\r", nameof(line));
        }
        byte[] data = Utf8.GetBytes(line + "\n");
        var clock = Stopwatch.StartNew();
        while (true)
        {
            try
            {
                using var file = new FileStream(path, FileMode.Append, FileAccess.Write, FileShare.None);
                file.Write(data);
                return;
            }
            // Only the exact type IOException tells that another writer holds the file.
            // DirectoryNotFoundException and the other subclasses go to the caller.
            catch (IOException e) when (e.GetType() == typeof(IOException) && clock.Elapsed.TotalSeconds < 10)
            {
                Thread.Sleep(1);
            }
        }
    }
}
